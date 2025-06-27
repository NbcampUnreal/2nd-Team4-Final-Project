#include "DW_NpcBase.h"
#include "DW_GameInstance.h"
#include "Character/DW_CharacterBase.h"
#include "UI/Component/QuestDatabase.h"
#include "UI/Component/Manager/QuestManagerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "UI/Widget/DialogueWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Components/ChildActorComponent.h"
#include "Engine/DataTable.h"

ADW_NpcBase::ADW_NpcBase()
{
	PrimaryActorTick.bCanEverTick = false;

	AppearanceComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("AppearanceComponent"));
	AppearanceComponent->SetupAttachment(RootComponent);

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
	if (!DialogueUIClass || !Interactor || !DialogueDataTable) return;
	APlayerController* PC = Cast<APlayerController>(Interactor->GetInstigatorController());
	if (PC)
	{
		UDialogueWidget* DialogueWidget = CreateWidget<UDialogueWidget>(PC, DialogueUIClass);
		if (DialogueWidget)
		{
			UQuestManagerComponent* QuestManager = Interactor->FindComponentByClass<UQuestManagerComponent>();
			TArray<FDialogueLine> FilteredLines = GetDialogueForQuestState(QuestManager);

			DialogueWidget->SetDialogueText(FilteredLines);

			if (bGivesQuest)
			{
				DialogueWidget->InitQuest(QuestID);
			}

			DialogueWidget->AddToViewport();
			PC->bShowMouseCursor = true;
			PC->SetInputMode(FInputModeUIOnly());

			FocusCameraOnNPC(Interactor);
		}
	}
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

void ADW_NpcBase::FocusCameraOnNPC(AActor* PlayerActor)
{
	if (!NPCInteractionCamera) return;

	APlayerController* PC = Cast<APlayerController>(PlayerActor->GetInstigatorController());
	if (PC)
	{
		PC->SetViewTargetWithBlend(NPCInteractionCamera, 1.0f);
	}
}

TArray<FDialogueLine> ADW_NpcBase::GetDialogueForQuestState(UQuestManagerComponent* QuestManager) const
{
	TArray<FDialogueLine> Result;

	if (!DialogueDataTable) return Result;

	// 퀘스트 연동 여부 판단 기본값 None
	bool bHasQuest = QuestID != NAME_None;
	EQuestDialogueStage Stage = EQuestDialogueStage::None;

	if (bGivesQuest && bHasQuest && QuestManager)
	{
		if (UDW_GameInstance* DWGI = Cast<UDW_GameInstance>(GetGameInstance()))
		{
			if (UQuestDatabase* QuestDB = DWGI->QuestDatabase)
			{
				FQuestData QuestData = QuestDB->FindQuestByID(QuestID);

				if (QuestData.QuestID != NAME_None)  // 유효한지 확인
				{
					if (QuestManager->IsQuestCompleted(QuestData))
						Stage = EQuestDialogueStage::Completed;
					else if (QuestManager->IsQuestAccepted(QuestData))
						Stage = EQuestDialogueStage::InProgress;
					else
						Stage = EQuestDialogueStage::BeforeAccept;
				}
			}
		}
	}

	static const FString Context = TEXT("NPC Dialogue");
	TArray<FDialogueLine*> AllRows;
	DialogueDataTable->GetAllRows(Context, AllRows);

	for (const FDialogueLine* Row : AllRows)
	{
		if (!Row) continue;

		// 퀘스트가 있는 NPC → 해당 QuestID와 상태가 맞는 대사만
		if (bHasQuest)
		{
			if (Row->QuestID == QuestID && Row->DialogueStage == Stage && Row->SpeakerName.ToString() == Name.ToString())
			{
				// 데이터 테이블에서 이름을 찾아서 대사 저장
				Result.Add(*Row);
			}
		}
		// 퀘스트 없는 NPC → DialogueStage == None만 허용
		else if (Row->DialogueStage == EQuestDialogueStage::None)
		{
			// 데이터 테이블에서 이름을 찾아서 대사 저장
			if (Row->SpeakerName.ToString() == Name.ToString())
			{
				Result.Add(*Row);
			}
		}
	}

	return Result;
}
