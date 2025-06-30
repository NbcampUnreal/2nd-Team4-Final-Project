#include "DW_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DW_SaveGame.h"
#include "Character/DW_CharacterBase.h"
#include "DW_LevelLoadSubsystem.h"
#include "UI/Widget/LoadingWidget.h"
#include "Item/ItemDataManager.h"
#include "DW_AttributeComponent.h"
#include "DW_SkillComponent.h"
#include "UI/Component/Manager/QuestManagerComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Engine/DataTable.h"
#include "Character/CharacterStatComponent.h"
#include "Character/CharacterArmorComponent.h"
#include "FogManager.h"
#include "UI/Widget/FogOfWarManager.h"
#include "UI/Widget/SettingsManager.h"

void UDW_GameInstance::Init()
{
    Super::Init();

    SettingsManager = NewObject<USettingsManager>(this);
    SettingsManager->Initialize();


    if (QuestDatabase && QuestDatabase->QuestDataTable)
    {
        UE_LOG(LogTemp, Log, TEXT("퀘스트 데이터베이스가 블루프린트에서 정상 설정되었습니다."));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("QuestDatabase 또는 QuestDataTable이 블루프린트에서 설정되지 않았습니다."));
    }

    // 1. UItemDataManager 인스턴스 생성
    ItemDataManager = NewObject<UItemDataManager>(this); // GameInstance를 Outer로 설정

    // 2. SetInstance를 통해 싱글턴 인스턴스 등록
    UItemDataManager::SetInstance(ItemDataManager);

    // 3. 데이터 테이블 로드 및 초기화
    if (!ItemBaseDataTableRef.IsNull())
    {
        UDataTable* LoadedDataTable = ItemBaseDataTableRef.LoadSynchronous(); // 동기 로딩
        if (LoadedDataTable)
        {
            ItemDataManager->InitializeDataManager(LoadedDataTable);
        }
        else
        {
#if WITH_EDITOR
            UE_LOG(LogTemp, Error, TEXT("Failed to load ItemBaseDataTable for UItemDataManager from %s"), *ItemBaseDataTableRef.GetAssetName());
#endif
        }
    }
    else
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Error, TEXT("ItemBaseDataTableRef is not set in GameInstance."));
#endif
    }
}

void UDW_GameInstance::Shutdown()
{

    // 게임 종료 시 싱글턴 인스턴스 해제
    UItemDataManager::SetInstance(nullptr); // SetInstance 내부에서 RemoveFromRoot 호출
    Super::Shutdown();
}

void UDW_GameInstance::SaveGameData()
{
    UDW_SaveGame* SaveGameInstance = Cast<UDW_SaveGame>(
        UGameplayStatics::CreateSaveGameObject(UDW_SaveGame::StaticClass())
    );
    if (!SaveGameInstance) return;
    
    if (SettingsManager)
    {
        SettingsManager->SaveSettingsTo(SaveGameInstance);
    }
    
    ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerCharacter) return;

    // 1. 위치 저장
    SaveGameInstance->SavedPlayerLocation = PlayerCharacter->GetActorLocation();

    // 2. 회전값 저장
    SaveGameInstance->SavedPlayerRotation = PlayerCharacter->GetActorRotation();

	// 3. 스탯 저장
	if (UCharacterStatComponent* StatComp = PlayerCharacter->FindComponentByClass<UCharacterStatComponent>())
	{
        FCharacterStatSaveData& Out = SaveGameInstance->SavedCharacterStat;

        Out.Health = StatComp->GetHealth();
        Out.Stamina = StatComp->GetStamina();
        Out.CurrentWeight = StatComp->GetCurrentWeight();

        Out.BaseMaxHealth = StatComp->GetBaseMaxHealth();
        Out.BaseHealthGenRate = StatComp->GetBaseHealthGenRate();
        Out.BaseMaxStamina = StatComp->GetBaseMaxStamina();
        Out.BaseStaminaGenRate = StatComp->GetBaseStaminaGenRate();
        Out.BaseAttack = StatComp->GetBaseAttack();
        Out.BaseDefense = StatComp->GetBaseDefense();
        Out.BaseAttackSpeed = StatComp->GetBaseAttackSpeed();
        Out.BaseWalkSpeed = StatComp->GetBaseWalkSpeed();
        Out.BaseMaxWeight = StatComp->GetBaseMaxWeight();

        Out.EquipmentBonusMaxHealth = StatComp->GetEquipmentBonusMaxHealth();
        Out.EquipmentBonusHealthGenRate = StatComp->GetEquipmentBonusHealthGenRate();
        Out.EquipmentBonusMaxStamina = StatComp->GetEquipmentBonusMaxStamina();
        Out.EquipmentBonusStaminaGenRate = StatComp->GetEquipmentBonusStaminaGenRate();
        Out.EquipmentBonusAttack = StatComp->GetEquipmentBonusAttack();
        Out.EquipmentBonusDefense = StatComp->GetEquipmentBonusDefense();
        Out.EquipmentBonusAttackSpeed = StatComp->GetEquipmentBonusAttackSpeed();
        Out.EquipmentBonusWalkSpeed = StatComp->GetEquipmentBonusWalkSpeed();
        Out.EquipmentBonusMaxWeight = StatComp->GetEquipmentBonusMaxWeight();
	}

    // 4. 스킬 관련 정보 저장(스킬포인트, 숙련도, 스킬트리)
    if (UDW_SkillComponent* SkillComp = PlayerCharacter->FindComponentByClass<UDW_SkillComponent>())
    {
        SaveGameInstance->SavedSkillData.CurrentSP = SkillComp->CurrentSP;
        SaveGameInstance->SavedSkillData.SkillStates = SkillComp->SkillStateMap;
        SaveGameInstance->SavedSkillData.LevelUpCount = SkillComp->LevelUpCount;
        SaveGameInstance->SavedSkillData.CurrentMastery = SkillComp->CurrentMastery;
        SaveGameInstance->SavedSkillData.MaxMastery = SkillComp->MaxMastery;
    }

	// 5. 퀘스트 상태 저장
    if (UQuestManagerComponent* QuestComp = PlayerCharacter->FindComponentByClass<UQuestManagerComponent>())
    {
        SaveGameInstance->SaveAcceptedQuests.Empty();
        SaveGameInstance->SaveCompletedQuestIDs.Empty();

        // 진행 중 퀘스트 저장
        for (const auto& Pair : QuestComp->AcceptedQuests)
        {
            const FQuestData& Quest = Pair.Value;

            FQuestProgressSaveData SaveData;
            SaveData.QuestID = Quest.QuestID;

            for (const FQuestObjective& Obj : Quest.Objectives)
            {
                SaveData.ObjectiveProgress.Add(Obj.CurrentCount);
            }

            SaveGameInstance->SaveAcceptedQuests.Add(SaveData);
        }

        // 완료 퀘스트 저장 (ID만)
        for (const auto& Pair : QuestComp->CompletedQuests)
        {
            SaveGameInstance->SaveCompletedQuestIDs.Add(Pair.Key);
        }
    }

    // 6. 인벤토리 저장
    if (UInventoryComponent* InvComp = PlayerCharacter->FindComponentByClass<UInventoryComponent>())
    {
        // 저장 배열 초기화
        SaveGameInstance->TempInventorySlotSaveDatas.Empty();

        // 슬롯 수 저장(안하니 한칸만 나옴)
        SaveGameInstance->SavedInventorySlotQuantity = InvComp->InventorySlotQuantity;

        for (const FInventorySlot& Slot : InvComp->InventorySlots)
        {
            if (Slot.ItemBase) // 유효한 아이템만 저장
            {
                FInventorySlotSaveData SaveSlot;
                SaveSlot.ItemCode = Slot.ItemBase->ItemCode;
                SaveSlot.Quantity = Slot.Quantity;

                SaveGameInstance->TempInventorySlotSaveDatas.Add(SaveSlot);
            }
        }
    }

	// 7. 방어구/무기 저장(아이템코드)
    if (UCharacterArmorComponent* ArmorComp = PlayerCharacter->FindComponentByClass<UCharacterArmorComponent>())
    {
        SaveGameInstance->SavedArmorData.HelmetCode = ArmorComp->Helmet ? ArmorComp->Helmet->ItemCode : "0";
        SaveGameInstance->SavedArmorData.ArmorCode = ArmorComp->Armor ? ArmorComp->Armor->ItemCode : "0";
        SaveGameInstance->SavedArmorData.GloveCode = ArmorComp->Glove ? ArmorComp->Glove->ItemCode : "0";
        SaveGameInstance->SavedArmorData.BootsCode = ArmorComp->Boots ? ArmorComp->Boots->ItemCode : "0";
        SaveGameInstance->SavedArmorData.WeaponCode = ArmorComp->Weapon ? ArmorComp->Weapon->ItemCode : "0";
    }

    // 8. 안개 저장
    SaveGameInstance->SavedFogMap = RevealedFogMap;

    UGameplayStatics::SaveGameToSlot(SaveGameInstance, DefaultSaveSlot, 0);
}

void UDW_GameInstance::LoadGameData()
{
    if (!UGameplayStatics::DoesSaveGameExist(DefaultSaveSlot, 0)) return;

    LoadedSaveGame = Cast<UDW_SaveGame>(UGameplayStatics::LoadGameFromSlot(DefaultSaveSlot, 0));
    if (!LoadedSaveGame) return;
}

void UDW_GameInstance::ApplyLoadedData()
{
    if (!LoadedSaveGame) return;

    if (SettingsManager)
    {
        SettingsManager->LoadSettingsFrom(LoadedSaveGame);
    }
    
    ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerCharacter) return;

    // 1. 위치 적용
    PlayerCharacter->SetActorLocation(LoadedSaveGame->SavedPlayerLocation);

    // 2. 회전값 적용
    PlayerCharacter->SetActorRotation(LoadedSaveGame->SavedPlayerRotation);

	// 4. 스탯 적용
	if (UCharacterStatComponent* StatComp = PlayerCharacter->FindComponentByClass<UCharacterStatComponent>())
	{
        const FCharacterStatSaveData& In = LoadedSaveGame->SavedCharacterStat;

        StatComp->SetStamina(In.Stamina);
        StatComp->SetCurrentWeight(In.CurrentWeight);

        StatComp->SetBaseMaxHealth(In.BaseMaxHealth);
        StatComp->SetBaseHealthGenRate(In.BaseHealthGenRate);
        StatComp->SetBaseMaxStamina(In.BaseMaxStamina);
        StatComp->SetBaseStaminaGenRate(In.BaseStaminaGenRate);
        StatComp->SetBaseAttack(In.BaseAttack);
        StatComp->SetBaseDefense(In.BaseDefense);
        StatComp->SetBaseAttackSpeed(In.BaseAttackSpeed);
        StatComp->SetBaseWalkSpeed(In.BaseWalkSpeed);
        StatComp->SetBaseMaxWeight(In.BaseMaxWeight);

        StatComp->SetEquipmentBonusMaxHealth(In.EquipmentBonusMaxHealth);
        StatComp->SetEquipmentBonusHealthGenRate(In.EquipmentBonusHealthGenRate);
        StatComp->SetEquipmentBonusMaxStamina(In.EquipmentBonusMaxStamina);
        StatComp->SetEquipmentBonusStaminaGenRate(In.EquipmentBonusStaminaGenRate);
        StatComp->SetEquipmentBonusAttack(In.EquipmentBonusAttack);
        StatComp->SetEquipmentBonusDefense(In.EquipmentBonusDefense);
        StatComp->SetEquipmentBonusAttackSpeed(In.EquipmentBonusAttackSpeed);
        StatComp->SetEquipmentBonusWalkSpeed(In.EquipmentBonusWalkSpeed);
        StatComp->SetEquipmentBonusMaxWeight(In.EquipmentBonusMaxWeight);

        StatComp->RecalculateAllTotalStats();
        StatComp->SetHealth(In.Health);
	}

    // 5. 스킬정보들 복원(스킬포인트, 스킬트리, 숙련도)
    if (UDW_SkillComponent* SkillComp = PlayerCharacter->FindComponentByClass<UDW_SkillComponent>())
    {
        const FSkillSystemSaveData& In = LoadedSaveGame->SavedSkillData;

        SkillComp->SkillStateMap = In.SkillStates;
        SkillComp->LevelUpCount = In.LevelUpCount;
        SkillComp->CurrentMastery = In.CurrentMastery;
        SkillComp->MaxMastery = In.MaxMastery;
        SkillComp->CurrentSP = In.CurrentSP;

        // 스킬 갱신
        if (UDW_AttributeComponent* AttrComp = PlayerCharacter->FindComponentByClass<UDW_AttributeComponent>())
        {
            SkillComp->ApplyAllSkillBonuses(AttrComp);
        }
    }

	// 6. 퀘스트 복원
    if (UQuestManagerComponent* QuestComp = PlayerCharacter->FindComponentByClass<UQuestManagerComponent>())
    {
        QuestComp->AcceptedQuests.Empty();
        QuestComp->CompletedQuests.Empty();

        // 진행 중 퀘스트 복원
        for (const FQuestProgressSaveData& SaveData : LoadedSaveGame->SaveAcceptedQuests)
        {
            FQuestData Quest = this->QuestDatabase->FindQuestByID(SaveData.QuestID);
            for (int32 i = 0; i < Quest.Objectives.Num() && i < SaveData.ObjectiveProgress.Num(); ++i)
            {
                Quest.Objectives[i].CurrentCount = SaveData.ObjectiveProgress[i];
            }

            QuestComp->AcceptedQuests.Add(Quest.QuestID, Quest);
        }

        // 완료 퀘스트 복원
        for (const FName& CompletedID : LoadedSaveGame->SaveCompletedQuestIDs)
        {
            FQuestData Quest = this->QuestDatabase->FindQuestByID(CompletedID);
            Quest.bIsCompleted = true;

            for (FQuestObjective& Obj : Quest.Objectives)
            {
                Obj.CurrentCount = Obj.RequiredCount; // 완료 처리
            }

            QuestComp->CompletedQuests.Add(Quest.QuestID, Quest);
        }
    }

	// 7. 인벤토리 복원
    if (UInventoryComponent* InvComp = PlayerCharacter->FindComponentByClass<UInventoryComponent>())
    {
        const auto& SavedSlots = LoadedSaveGame->TempInventorySlotSaveDatas;

        // 슬롯 수 복원
        InvComp->InventorySlotQuantity = LoadedSaveGame->SavedInventorySlotQuantity;

        InvComp->InventorySlotQuantity = SavedSlots.Num();
        InvComp->InventorySlots.SetNum(SavedSlots.Num());

        for (int32 i = 0; i < SavedSlots.Num(); ++i)
        {
            const FInventorySlotSaveData& SavedSlot = SavedSlots[i];
            FInventorySlot& TargetSlot = InvComp->InventorySlots[i];

            // 아이템 인스턴스 생성
            UItemBase* NewItem = NewObject<UItemBase>(InvComp);
            if (NewItem)
            {
                // 핵심: ItemCode를 통해 데이터 테이블 기반 정보 로드
                NewItem->LoadItemFromCode(SavedSlot.ItemCode);

                TargetSlot.ItemBase = NewItem;
                TargetSlot.Quantity = SavedSlot.Quantity;
                TargetSlot.bIsOccupied = true;
            }
        }
    }

	// 8. 방어구/무기 복원
    if (UCharacterArmorComponent* ArmorComp = PlayerCharacter->FindComponentByClass<UCharacterArmorComponent>())
    {
        const FEquippedArmorSaveData& Loaded = LoadedSaveGame->SavedArmorData;

        auto CreateItem = [&](FString Code) -> UItemBase*
            {
                if (!Code.IsEmpty()) return nullptr;

                UItemBase* NewItem = NewObject<UItemBase>(ArmorComp);
                if (NewItem)
                {
                    NewItem->LoadItemFromCode(Code);
                }
                return NewItem;
            };

        ArmorComp->Helmet = CreateItem(Loaded.HelmetCode);
        ArmorComp->Armor = CreateItem(Loaded.ArmorCode);
        ArmorComp->Glove = CreateItem(Loaded.GloveCode);
        ArmorComp->Boots = CreateItem(Loaded.BootsCode);
        ArmorComp->Weapon = CreateItem(Loaded.WeaponCode);

        //필수!! 캐릭터에 장비로 스탯 올라가는 함수 추가되고 나면은 여기 추가해주기!!
        //근데 스탯에서 다 저장해서 없어도 될꺼같은데 확인해야함

		// 캐릭터의 스켈레탈 메시 업데이트
        PlayerCharacter->UpdateSkeletalMesh();
    }

    // 9. 안개 적용
    RevealedFogMap = LoadedSaveGame->SavedFogMap;

    LoadedSaveGame = nullptr; // 일회성 데이터로 초기화
}

void UDW_GameInstance::CacheTempDataBeforeLevelChange()
{
    ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Player) return;
    // 던전 들어갔다 나올때 위치?
    CachedTempSaveData.PlayerLocation = Player->GetActorLocation();
    CachedTempSaveData.PlayerRotation = Player->GetActorRotation();

    if (auto* Attr = Player->FindComponentByClass<UDW_AttributeComponent>())
    {
        Attr->SaveData(CachedTempSaveData.TempAttributes);
    }

    if (auto* Skill = Player->FindComponentByClass<UDW_SkillComponent>())
    {
        CachedTempSaveData.TempSkillStates = Skill->SkillStateMap;
    }

    if (auto* Quest = Player->FindComponentByClass<UQuestManagerComponent>())
    {
        CachedTempSaveData.TempAcceptedQuests = Quest->GetActiveQuests();
        CachedTempSaveData.TempCompletedQuests = Quest->GetCompletedQuests();
    }

    if (auto* Inv = Player->FindComponentByClass<UInventoryComponent>())
    {
        CachedTempSaveData.TempInventory.TempInventorySlots = Inv->InventorySlots;
        CachedTempSaveData.TempInventory.TempInventorySlotQuantity = Inv->InventorySlotQuantity;

        // SoftObjectPtr로 변환
        CachedTempSaveData.TempInventory.TempEquippedItems.Empty();
        for (const auto& Pair : Inv->EquippedItems)
        {
            CachedTempSaveData.TempInventory.TempEquippedItems.Add(Pair.Key, TSoftObjectPtr<UEquippableItem>(Pair.Value));
        }
    }

    if (auto* Armor = Player->FindComponentByClass<UCharacterArmorComponent>())
    {
        CachedTempSaveData.TempArmor.Helmet = TSoftObjectPtr<UItemBase>(Armor->Helmet);
        CachedTempSaveData.TempArmor.Armor = TSoftObjectPtr<UItemBase>(Armor->Armor);
        CachedTempSaveData.TempArmor.Glove = TSoftObjectPtr<UItemBase>(Armor->Glove);
        CachedTempSaveData.TempArmor.Boots = TSoftObjectPtr<UItemBase>(Armor->Boots);
        CachedTempSaveData.TempArmor.Weapon = TSoftObjectPtr<UItemBase>(Armor->Weapon);
    }
}

void UDW_GameInstance::ApplyTempDataAfterLevelLoad()
{
    ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Player) return;

	// Component들에 데이터 적용
    ApplyTempDatatoCharacterComponents(Player);
}

void UDW_GameInstance::ApplyTempDataAfterLevelLoadWithLocation()
{
    ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Player) return;

    // 위치와 회전값 적용
	Player->SetActorLocation(CachedTempSaveData.PlayerLocation);
	Player->SetActorRotation(CachedTempSaveData.PlayerRotation);
	
    // Component들에 데이터 적용
	ApplyTempDatatoCharacterComponents(Player);
}

void UDW_GameInstance::ApplyTempDatatoCharacterComponents(ADW_CharacterBase* Player)
{
	check(Player); // Player가 nullptr이 아니어야 함

    if (auto* Attr = Player->FindComponentByClass<UDW_AttributeComponent>())
    {
        Attr->LoadData(CachedTempSaveData.TempAttributes);
        if (auto* Skill = Player->FindComponentByClass<UDW_SkillComponent>())
        {
            Skill->SkillStateMap = CachedTempSaveData.TempSkillStates;
            Skill->ApplyAllSkillBonuses(Attr);
        }
    }

    if (auto* Stat = Player->FindComponentByClass<UCharacterStatComponent>())
    {
        const auto& In = CachedTempSaveData.TempStatData;
        Stat->SetHealth(In.Health);
        Stat->SetStamina(In.Stamina);
        Stat->SetCurrentWeight(In.CurrentWeight);

        Stat->SetBaseMaxHealth(In.BaseMaxHealth);
        Stat->SetBaseHealthGenRate(In.BaseHealthGenRate);
        Stat->SetBaseMaxStamina(In.BaseMaxStamina);
        Stat->SetBaseStaminaGenRate(In.BaseStaminaGenRate);
        Stat->SetBaseAttack(In.BaseAttack);
        Stat->SetBaseDefense(In.BaseDefense);
        Stat->SetBaseAttackSpeed(In.BaseAttackSpeed);
        Stat->SetBaseWalkSpeed(In.BaseWalkSpeed);
        Stat->SetBaseMaxWeight(In.BaseMaxWeight);

        Stat->SetEquipmentBonusMaxHealth(In.EquipmentBonusMaxHealth);
        Stat->SetEquipmentBonusHealthGenRate(In.EquipmentBonusHealthGenRate);
        Stat->SetEquipmentBonusMaxStamina(In.EquipmentBonusMaxStamina);
        Stat->SetEquipmentBonusStaminaGenRate(In.EquipmentBonusStaminaGenRate);
        Stat->SetEquipmentBonusAttack(In.EquipmentBonusAttack);
        Stat->SetEquipmentBonusDefense(In.EquipmentBonusDefense);
        Stat->SetEquipmentBonusAttackSpeed(In.EquipmentBonusAttackSpeed);
        Stat->SetEquipmentBonusWalkSpeed(In.EquipmentBonusWalkSpeed);
        Stat->SetEquipmentBonusMaxWeight(In.EquipmentBonusMaxWeight);
    }

    if (auto* Quest = Player->FindComponentByClass<UQuestManagerComponent>())
    {
        Quest->AcceptedQuests = CachedTempSaveData.TempAcceptedQuests;
        Quest->CompletedQuests = CachedTempSaveData.TempCompletedQuests;
    }

    if (auto* Inv = Player->FindComponentByClass<UInventoryComponent>())
    {
        Inv->InventorySlots = CachedTempSaveData.TempInventory.TempInventorySlots;
        Inv->InventorySlotQuantity = CachedTempSaveData.TempInventory.TempInventorySlotQuantity;

        Inv->EquippedItems.Empty();
        for (const auto& Pair : CachedTempSaveData.TempInventory.TempEquippedItems)
        {
            if (Pair.Value.IsValid())
            {
                Inv->EquippedItems.Add(Pair.Key, Pair.Value.Get());
            }
        }
    }

    if (auto* Armor = Player->FindComponentByClass<UCharacterArmorComponent>())
    {
        Armor->Helmet = CachedTempSaveData.TempArmor.Helmet.Get();
        Armor->Armor = CachedTempSaveData.TempArmor.Armor.Get();
        Armor->Glove = CachedTempSaveData.TempArmor.Glove.Get();
        Armor->Boots = CachedTempSaveData.TempArmor.Boots.Get();
        Armor->Weapon = CachedTempSaveData.TempArmor.Weapon.Get();
        Player->UpdateSkeletalMesh();   // 장비 변경 후 스켈레탈 메시 업데이트
    }
}

void UDW_GameInstance::RevealGrid(FName MapName, int32 X, int32 Y)
{
    FBitmaskFogData& FogData = RevealedFogMap.FindOrAdd(MapName);

    // 비트마스크 크기 초기화 (처음 등록 시)
    if (FogData.Bitmask.Num() == 0)
    {
        FogData.Width = 128;  // 실제 사용하는 GridCountX로 교체
        FogData.Height = 128; // 실제 사용하는 GridCountY로 교체

        int32 TotalBits = FogData.Width * FogData.Height;
        int32 TotalBytes = FMath::DivideAndRoundUp(TotalBits, 8);
        FogData.Bitmask.Init(0, TotalBytes);
    }

    int32 Index = Y * FogData.Width + X;
    int32 ByteIndex = Index / 8;
    int32 BitIndex = Index % 8;

    if (FogData.Bitmask.IsValidIndex(ByteIndex))
    {
        FogData.Bitmask[ByteIndex] |= (1 << BitIndex);
    }
}

void UDW_GameInstance::GetRevealedGridSet(FName MapName, TSet<FIntPoint>& OutGridSet) const
{
    const FBitmaskFogData* FogData = RevealedFogMap.Find(MapName);
    if (!FogData) return;

    int32 TotalBits = FogData->Width * FogData->Height;
    for (int32 Index = 0; Index < TotalBits; ++Index)
    {
        int32 ByteIndex = Index / 8;
        int32 BitIndex = Index % 8;

        if ((FogData->Bitmask[ByteIndex] >> BitIndex) & 1)
        {
            int32 X = Index % FogData->Width;
            int32 Y = Index / FogData->Width;
            OutGridSet.Add(FIntPoint(X, Y));
        }
    }
}
