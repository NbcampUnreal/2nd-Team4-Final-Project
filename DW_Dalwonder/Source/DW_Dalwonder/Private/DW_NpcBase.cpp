#include "DW_NpcBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

ADW_NpcBase::ADW_NpcBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// 메쉬 설정
	MeshComponent = GetMesh();

	// 위젯 컴포넌트 초기화
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(RootComponent);
	InteractionWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractionWidget->SetDrawAtDesiredSize(true);
	InteractionWidget->SetVisibility(false);

	// 기본 위젯 클래스 할당 (선택적)
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/UI/WBP_InteractPrompt"));
	if (WidgetClassFinder.Succeeded())
	{
		InteractionWidgetClass = WidgetClassFinder.Class;
	}
}

void ADW_NpcBase::BeginPlay()
{
	Super::BeginPlay();

	if (InteractionWidget && InteractionWidgetClass)
	{
		InteractionWidget->SetWidgetClass(InteractionWidgetClass);
	}
}

void ADW_NpcBase::Interact_Implementation(AActor* Interactor)
{
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("NPC와 상호작용 발생: %s"), *GetName());
#endif
	// 여기서 대화 UI 띄우기 또는 퀘스트 진행 로직 호출
}

void ADW_NpcBase::ShowInteractionWidget_Implementation()
{
	if (InteractionWidget && InteractionWidgetClass)
	{
		InteractionWidget->SetVisibility(true);
	}
	else
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("InteractionWidget 또는 InteractionWidgetClass가 null입니다."));
#endif
	}
}

void ADW_NpcBase::HideInteractionWidget_Implementation()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(false);
	}
}
