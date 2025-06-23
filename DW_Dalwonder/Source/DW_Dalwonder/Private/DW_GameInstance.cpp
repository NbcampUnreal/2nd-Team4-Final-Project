#include "DW_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DW_SaveGame.h"
#include "Character/DW_CharacterBase.h"
#include "DW_LevelLoadSubsystem.h"
#include "UI/Widget/LoadingWidget.h"
#include "Item/ItemDataManager.h"

void UDW_GameInstance::Init()
{
    Super::Init();

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

    ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerCharacter) return;

    // 1. 위치 저장
    SaveGameInstance->SavedPlayerLocation = PlayerCharacter->GetActorLocation();

    // 2. 회전값 저장
    SaveGameInstance->SavedPlayerRotation = PlayerCharacter->GetActorRotation();

    // 3. 스탯 저장
    if (UDW_AttributeComponent* AttrComp = PlayerCharacter->FindComponentByClass<UDW_AttributeComponent>())
    {
        AttrComp->SaveData(SaveGameInstance->SavedAttributes);
    }

    // 4. 스킬 트리 저장
    if (UDW_SkillComponent* SkillComp = PlayerCharacter->FindComponentByClass<UDW_SkillComponent>())
    {
        SaveGameInstance->SavedSkillStates = SkillComp->SkillStateMap;
    }

    UGameplayStatics::SaveGameToSlot(SaveGameInstance, DefaultSaveSlot, 0);
}

void UDW_GameInstance::LoadGameData()
{
    if (!UGameplayStatics::DoesSaveGameExist(DefaultSaveSlot, 0)) return;

    LoadedSaveGame = Cast<UDW_SaveGame>(UGameplayStatics::LoadGameFromSlot(DefaultSaveSlot, 0));
    if (!LoadedSaveGame) return;

    UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestLoadingMap"));
}

void UDW_GameInstance::ApplyLoadedData()
{
    if (!LoadedSaveGame) return;

    ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!PlayerCharacter) return;

    // 1. 위치 적용
    PlayerCharacter->SetActorLocation(LoadedSaveGame->SavedPlayerLocation);

    // 2. 회전값 적용
    PlayerCharacter->SetActorRotation(LoadedSaveGame->SavedPlayerRotation);

    // 3. Attribute 적용
    UDW_AttributeComponent* AttrComp = PlayerCharacter->FindComponentByClass<UDW_AttributeComponent>();
    if (AttrComp)
    {
        AttrComp->LoadData(LoadedSaveGame->SavedAttributes);
    }

    // 4. Skill 복원 + 보너스 적용
    if (UDW_SkillComponent* SkillComp = PlayerCharacter->FindComponentByClass<UDW_SkillComponent>())
    {
        SkillComp->SkillStateMap = LoadedSaveGame->SavedSkillStates;
        if (AttrComp)
        {
            SkillComp->ApplyAllSkillBonuses(AttrComp);
        }
    }

    LoadedSaveGame = nullptr; // 일회성 데이터로 초기화
}

void UDW_GameInstance::LoadLevelWithLoadingScreen(FName LevelName)
{
    // 서브시스템 가져오기
    UDW_LevelLoadSubsystem* LoadSubsystem = GetSubsystem<UDW_LevelLoadSubsystem>();
    if (!LoadSubsystem) return;
    
    if (!LoadingWidgetClass) return;

    // 로딩 위젯 넘겨주고 맵 비동기 로드 시작
    LoadSubsystem->SetLoadingWidgetClass(LoadingWidgetClass);
    LoadSubsystem->StreamLevelAsync(LevelName);
}

void UDW_GameInstance::StartLevelStreaming()
{
    if (!LevelLoadSubsystem)
    {
        LevelLoadSubsystem = GetSubsystem<UDW_LevelLoadSubsystem>();
    }

    // 서브시스템로드실패 및 맵이름 없을때
    if (!LevelLoadSubsystem || PendingLevelName.IsNone()) return;

    LevelLoadSubsystem->StreamLevelAsync(PendingLevelName);
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
            CachedTempSaveData.TempInventory.TempEquippedItems.Add(Pair.Key, TSoftObjectPtr<UItemBase>(Pair.Value));
        }
    }
}

void UDW_GameInstance::ApplyTempDataAfterLevelLoad()
{
    ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Player) return;

    Player->SetActorLocation(CachedTempSaveData.PlayerLocation);
    Player->SetActorRotation(CachedTempSaveData.PlayerRotation);

    if (auto* Attr = Player->FindComponentByClass<UDW_AttributeComponent>())
    {
        Attr->LoadData(CachedTempSaveData.TempAttributes);
        if (auto* Skill = Player->FindComponentByClass<UDW_SkillComponent>())
        {
            Skill->SkillStateMap = CachedTempSaveData.TempSkillStates;
            Skill->ApplyAllSkillBonuses(Attr);
        }
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

        // 장비 스탯 반영
        Inv->UpdateEquippedStats();
    }
}

void UDW_GameInstance::ApplyTempDataAfterLevelLoadSecond()
{
    ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (!Player) return;

    if (auto* Attr = Player->FindComponentByClass<UDW_AttributeComponent>())
    {
        Attr->LoadData(CachedTempSaveData.TempAttributes);
        if (auto* Skill = Player->FindComponentByClass<UDW_SkillComponent>())
        {
            Skill->SkillStateMap = CachedTempSaveData.TempSkillStates;
            Skill->ApplyAllSkillBonuses(Attr);
        }
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

        // 장비 스탯 반영
        Inv->UpdateEquippedStats();
    }
}