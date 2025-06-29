#include "DW_GmBase.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "DW_GameInstance.h"
#include "DW_PortalArrivalActor.h"
#include "UI/Widget/ResultWidget.h"
#include "DW_SaveGame.h"
#include "Components/Image.h"
#include "GameFramework/Character.h"
#include "UI/Widget/FogOfWarManager.h"
#include "UI/Widget/SettingsManager.h"
#include "DW_SkillManager.h"

ADW_GmBase::ADW_GmBase()
{
    CurrentWidget = nullptr;
}

void ADW_GmBase::BeginPlay()
{
    Super::BeginPlay();

    if (UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>())
    {
        if (USettingsManager* SM = GI->GetSettingsManager())
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(TimerHandle, [SM]()
            {
                SM->ApplyVolumeMaster(SM->GetVolumeMaster());
                SM->ApplyVolumeBGM(SM->GetVolumeBGM());
                SM->ApplyVolumeSFX(SM->GetVolumeSFX());
                SM->ApplyVolumeUI(SM->GetVolumeUI());
            }, 0.1f, false);
        }
    }
    
    if (!SkillManager)
    {
        SkillManager = NewObject<UDW_SkillManager>(this);
        // SkillDataTable 할당
        SkillManager->Initialize(SkillDataTable);
    }

    UWorld* World = GetWorld();
    if (!World) return;

    UDW_GameInstance* GI = GetGameInstance<UDW_GameInstance>();
    if (!GI) return;

    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(World, 0);
    if (!Player) return;

    TArray<AActor*> FoundPortals;
    UGameplayStatics::GetAllActorsOfClass(World, ADW_PortalArrivalActor::StaticClass(), FoundPortals);

    for (AActor* Actor : FoundPortals)
    {
        ADW_PortalArrivalActor* Portal = Cast<ADW_PortalArrivalActor>(Actor);
        if (Portal && Portal->PortalType == GI->LastPortalType)
        {
            Player->SetActorLocation(Portal->GetActorLocation());
            Player->SetActorRotation(Portal->GetActorRotation());
            break;
        }
    }

    GI->LastPortalType = EPortalType::None;


    //안개

    // FogManager = GetWorld()->SpawnActor<AFogOfWarManager>();
    //
    // APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    // if (PC && PC->GetPawn())
    // {
    //     FVector Location = PC->GetPawn()->GetActorLocation();
    //     FogManager->GridOrigin = FVector2d(Location.X, Location.Y);
    //     FogManager->InitFog();
    // }
    //
    // FogMaterialInstance = UMaterialInstanceDynamic::Create(FogMaterialBase, this);
    // if (!FogMaterialInstance)
    // {
    //     return;
    // }
    //
    // FogMaterialInstance->SetTextureParameterValue("FogTexture", FogManager->GetFogTexture());
    //
    // if (FogOverlayWidgetClass)
    // {
    //     FogWidget = CreateWidget<UUserWidget>(GetWorld(), FogOverlayWidgetClass);
    //     if (FogWidget)
    //     {
    //         FogWidget->AddToViewport();
    //
    //         UImage* FogImage = Cast<UImage>(FogWidget->GetWidgetFromName(TEXT("FogImage")));
    //         if (FogImage && FogMaterialInstance)
    //         {
    //             FogImage->SetBrushFromMaterial(FogMaterialInstance);
    //         }
    //     }
    // }

    //안개
}

void ADW_GmBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ADW_GmBase::SwitchUI(TSubclassOf<UUserWidget> NewWidgetClass)
{
    if (CurrentWidget)
    {
        CurrentWidget->RemoveFromParent();
        CurrentWidget = nullptr;
    }

    if (NewWidgetClass)
    {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
        if (CurrentWidget)
        {
            CurrentWidget->AddToViewport();
        }
    }
}

UUserWidget* ADW_GmBase::ShowPopupUI(TSubclassOf<UUserWidget> WidgetClass)
{
    if (!WidgetClass) return nullptr;

    // 위젯 생성
    UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
    if (!NewWidget) return nullptr;

    // 화면에 추가
    NewWidget->AddToViewport(10);
    PopupWidgets.Add(NewWidget);

    // 입력‧마우스 세팅
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PC->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(NewWidget->TakeWidget());
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);
        PC->SetInputMode(InputMode);
    }

    return NewWidget;   // 반환
}

UUserWidget* ADW_GmBase::ShowPopupUI_M(TSubclassOf<UUserWidget> WidgetClass)
{
    if (!WidgetClass) return nullptr;

    // 위젯 생성
    UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
    if (!NewWidget) return nullptr;

    // 화면에 추가
    NewWidget->AddToViewport(10);
    PopupWidgets.Add(NewWidget);

    // 입력‧마우스 세팅
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PC->bShowMouseCursor = false;

        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }

    return NewWidget;   // 반환
}


void ADW_GmBase::ClosePopupUI(UUserWidget* WidgetToRemove)
{
    if (!WidgetToRemove) return;

    WidgetToRemove->RemoveFromParent();
    PopupWidgets.Remove(WidgetToRemove);

    if (PopupWidgets.Num() == 0)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            PC->bShowMouseCursor = false;
            FInputModeGameOnly InputMode;
            PC->SetInputMode(InputMode);
        }
    }
}

void ADW_GmBase::CloseLastPopupUI()
{
    if (PopupWidgets.Num() == 0)
        return;

    UUserWidget* LastWidget = PopupWidgets.Last();
    if (LastWidget)
    {
        LastWidget->RemoveFromParent();
        PopupWidgets.Pop();
    }

    if (PopupWidgets.Num() == 0)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            PC->bShowMouseCursor = false;

            FInputModeGameAndUI InputMode;
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
        }
    }
}

UUserWidget* ADW_GmBase::CloseLastPopupUI_AndReturn()
{
    if (PopupWidgets.Num() == 0)
        return nullptr;

    UUserWidget* LastWidget = PopupWidgets.Last();
    if (LastWidget)
    {
        LastWidget->RemoveFromParent();
        PopupWidgets.Pop();

        // 마우스 커서와 InputMode도 필요한 경우 여기서 처리
        if (PopupWidgets.Num() == 0)
        {
            if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
            {
                PC->SetShowMouseCursor(false);
                PC->SetInputMode(FInputModeGameOnly());
            }
        }
    }

    return LastWidget;
}

void ADW_GmBase::ShowResultUI(const FString& MessageText)
{
    if (!ResultWidgetClass) return;

    UResultWidget* ResultUI = Cast<UResultWidget>(ShowPopupUI_M(ResultWidgetClass));
    if (ResultUI)
    {
        ResultUI->SetResultText(MessageText);
        ResultUI->StartLetterSpacingAnimation();

        if (ResultUI->ResultTextAnimation)
        {
            ResultUI->PlayAnimation(ResultUI->ResultTextAnimation);
            ResultUI->PlayAnimation(ResultUI->ResultTextShadowAnimation);
        }

        FTimerHandle Handle;
        GetWorld()->GetTimerManager().SetTimer(Handle, [=, this]()
        {
            this->ClosePopupUI(ResultUI);
        }, 3.f, false);
    }
}
