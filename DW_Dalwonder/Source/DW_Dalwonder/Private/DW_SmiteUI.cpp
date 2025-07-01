#include "DW_SmiteUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"
#include "TimerManager.h"
#include "DW_GmBase.h"
#include "NPC/DW_NPC_Blacksmith.h"

void UDW_SmiteUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Smite)
	{
		Button_Smite->OnClicked.AddDynamic(this, &UDW_SmiteUI::OnSmiteButtonClicked);
	}

	if (Button_Craft)
	{
		Button_Craft->OnClicked.AddDynamic(this, &UDW_SmiteUI::OnCraftButtonClicked);
	}
	
	if (Button_Exit)
	{
		Button_Exit->OnClicked.AddDynamic(this, &UDW_SmiteUI::OnExitButtonClicked);
	}
}

void UDW_SmiteUI::OnCraftButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);

	if (ADW_NPC_Blacksmith* Blacksmith = Cast<ADW_NPC_Blacksmith>(RelatedNPC))
	{
		Blacksmith->HandleCraftAction();
	}

	GetWorld()->GetTimerManager().SetTimer(UnhideUITimerHandle, this, &UDW_SmiteUI::UnhideUI, 2.5f, false);
}


void UDW_SmiteUI::OnSmiteButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);

	if (ADW_NPC_Blacksmith* Blacksmith = Cast<ADW_NPC_Blacksmith>(RelatedNPC))
	{
		const bool bSuccess = Blacksmith->HandleEnhanceLogic();
		if (bSuccess)
		{
			Blacksmith->HandleEnhancementAction();
			PlaySequence(SuccessSequence);
		}
		else
		{
			PlaySequence(FailSequence);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(UnhideUITimerHandle, this, &UDW_SmiteUI::UnhideUI, 2.5f, false);
}

void UDW_SmiteUI::PlaySequence(ULevelSequence* Sequence)
{
	if (!Sequence) return;

	ALevelSequenceActor* OutActor;
	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = true;

	ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Sequence, Settings, OutActor);
	if (Player)
	{
		Player->Play();
	}
}

void UDW_SmiteUI::UnhideUI()
{
	this->SetVisibility(ESlateVisibility::Visible);
}

void UDW_SmiteUI::OnExitButtonClicked()
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
		{
			GM->ClosePopupUI(this);
		}
	}
}