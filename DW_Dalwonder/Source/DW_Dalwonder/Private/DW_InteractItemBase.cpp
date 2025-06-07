#include "DW_InteractItemBase.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"

ADW_InteractItemBase::ADW_InteractItemBase()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    // 위젯 컴포넌트 초기화
    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(RootComponent);
    InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
    InteractionWidget->SetDrawAtDesiredSize(true);
    InteractionWidget->SetVisibility(false);  // 시작 시 숨김 처리

    // InteractionWidgetClass가 제대로 할당된 경우 위젯 클래스 설정
    static ConstructorHelpers::FClassFinder<UUserWidget> InteractionWidgetClassFinder(TEXT("/Game/UI/WBP_InteractPrompt"));
    if (InteractionWidgetClassFinder.Succeeded())
    {
        InteractionWidgetClass = InteractionWidgetClassFinder.Class;
    }
}

void ADW_InteractItemBase::ShowInteractionWidget_Implementation()
{
    if (InteractionWidget && InteractionWidgetClass)
    {
        // InteractionWidget에 클래스 설정
        InteractionWidget->SetWidgetClass(InteractionWidgetClass);
        InteractionWidget->SetVisibility(true);
    }
    else
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Error, TEXT("InteractionWidget 또는 InteractionWidgetClass가 null입니다."));
#endif
	}
}

void ADW_InteractItemBase::HideInteractionWidget_Implementation()
{
    if (InteractionWidget)
    {
        InteractionWidget->SetVisibility(false);
    }
}

void ADW_InteractItemBase::Interact_Implementation(AActor* Interactor)
{
    if (Interactor)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("상호작용이 발생했습니다! Interactor: %s"), *Interactor->GetName());
#endif
	}
    else
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("상호작용이 발생했지만, Interactor가 유효하지 않습니다."));
#endif
	}
}