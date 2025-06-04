#include "DW_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "DW_SaveGame.h"
#include "Character/DW_CharacterBase.h"
#include "DW_LevelLoadSubsystem.h"
#include "UI/Widget/LoadingWidget.h"

void UDW_GameInstance::Init()
{
    Super::Init();

}

void UDW_GameInstance::Shutdown()
{
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

    // 2. 스탯 저장
    if (UDW_AttributeComponent* AttrComp = PlayerCharacter->FindComponentByClass<UDW_AttributeComponent>())
    {
        AttrComp->SaveData(SaveGameInstance->SavedAttributes);
    }

    // 3. 스킬 트리 저장
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

    // 2. Attribute 적용
    UDW_AttributeComponent* AttrComp = PlayerCharacter->FindComponentByClass<UDW_AttributeComponent>();
    if (AttrComp)
    {
        AttrComp->LoadData(LoadedSaveGame->SavedAttributes);
    }

    // 3. Skill 복원 + 보너스 적용
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
    UDW_LevelLoadSubsystem* LoadSubsystem = GetSubsystem<UDW_LevelLoadSubsystem>();
    if (!LoadSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT(" LevelLoadSubsystem 찾기 실패"));
        return;
    }

    if (!LoadingWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT(" GameInstance에 LoadingWidgetClass가 설정되지 않았습니다."));
        return;
    }

    LoadSubsystem->SetLoadingWidgetClass(LoadingWidgetClass);
    LoadSubsystem->StreamLevelAsync(LevelName);

    //UWorld* World = GetWorld();
    //if (!World)
    //{
    //    UE_LOG(LogTemp, Error, TEXT(" UDW_GameInstance::GetWorld() == nullptr"));
    //    return;
    //}

    //if (!LoadingWidgetClass)
    //{
    //    UE_LOG(LogTemp, Error, TEXT(" LoadingWidgetClass가 설정되지 않았습니다."));
    //    return;
    //}

    //// 위젯 생성
    //LoadingWidget = CreateWidget<ULoadingWidget>(World, LoadingWidgetClass);
    //if (!LoadingWidget)
    //{
    //    UE_LOG(LogTemp, Error, TEXT(" 로딩 위젯 생성 실패"));
    //    return;
    //}

    //LoadingWidget->AddToViewport();
    //LoadingWidget->UpdateProgress(0.f);

    //// Subsystem 접근 및 설정
    //if (UDW_LevelLoadSubsystem* Sub = GetSubsystem<UDW_LevelLoadSubsystem>())
    //{
    //    Sub->SetLoadingWidgetClass(LoadingWidgetClass);
    //    Sub->OnProgressUpdated.AddUObject(this, &UDW_GameInstance::HandleProgressUpdated);
    //    Sub->OnLoadingFinished.AddUObject(this, &UDW_GameInstance::HandleLoadingFinished);

    //    Sub->StartLoadingLevel(LevelName);

    //    // Tick 시작
    //    World->GetTimerManager().SetTimer(
    //        LoadingTickHandle,
    //        this,
    //        &UDW_GameInstance::TickLoading,
    //        0.05f,
    //        true
    //    );

    //    UE_LOG(LogTemp, Log, TEXT(" 로딩 시작: %s"), *LevelName.ToString());
    //}
}

//void UDW_GameInstance::HandleProgressUpdated(float Progress)
//{
//    if (LoadingWidget)
//    {
//        UE_LOG(LogTemp, Warning, TEXT(" HandleProgressUpdated: %f"), Progress);
//        LoadingWidget->UpdateProgress(Progress);
//    }
//}
//
//void UDW_GameInstance::HandleLoadingFinished()
//{
//    if (UWorld* World = GetWorld())
//    {
//        World->GetTimerManager().ClearTimer(LoadingTickHandle);
//    }
//
//    if (LoadingWidget)
//    {
//        LoadingWidget->RemoveFromParent();
//        LoadingWidget = nullptr;
//    }
//
//    if (UDW_LevelLoadSubsystem* Sub = GetSubsystem<UDW_LevelLoadSubsystem>())
//    {
//        FString MapToOpen = Sub->GetPendingLevelName();
//        UGameplayStatics::OpenLevel(this, FName(*MapToOpen));
//    }
//}
//
//void UDW_GameInstance::TickLoading()
//{
//    if (UDW_LevelLoadSubsystem* Sub = GetSubsystem<UDW_LevelLoadSubsystem>())
//    {
//        Sub->InternalTick(); // 진행률 추적
//    }
//}

