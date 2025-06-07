#include "DW_InteractItemBase.h"
#include "Components/WidgetComponent.h"
#include "UObject/ConstructorHelpers.h"

ADW_InteractItemBase::ADW_InteractItemBase()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    // ���� ������Ʈ �ʱ�ȭ
    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(RootComponent);
    InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
    InteractionWidget->SetDrawAtDesiredSize(true);
    InteractionWidget->SetVisibility(false);  // ���� �� ���� ó��

    // InteractionWidgetClass�� ����� �Ҵ�� ��� ���� Ŭ���� ����
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
        // InteractionWidget�� Ŭ���� ����
        InteractionWidget->SetWidgetClass(InteractionWidgetClass);
        InteractionWidget->SetVisibility(true);
    }
    else
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Error, TEXT("InteractionWidget �Ǵ� InteractionWidgetClass�� null�Դϴ�."));
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
        UE_LOG(LogTemp, Warning, TEXT("��ȣ�ۿ��� �߻��߽��ϴ�! Interactor: %s"), *Interactor->GetName());
#endif
	}
    else
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("��ȣ�ۿ��� �߻�������, Interactor�� ��ȿ���� �ʽ��ϴ�."));
#endif
	}
}