#include "DW_SmiteUI.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"
#include "TimerManager.h"
#include "DW_GmBase.h"

void UDW_SmiteUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Smite)
	{
		Button_Smite->OnClicked.AddDynamic(this, &UDW_SmiteUI::OnSmiteButtonClicked);
	}
	if (Button_Exit)
	{
		Button_Exit->OnClicked.AddDynamic(this, &UDW_SmiteUI::OnExitButtonClicked);
	}
}

void UDW_SmiteUI::OnSmiteButtonClicked()
{
	// UI 숨기기
	this->SetVisibility(ESlateVisibility::Hidden);

	// 강화 성공/실패 랜덤 판정
	const bool bSuccess = FMath::RandBool();
	if (bSuccess && SuccessSequence)
	{
		PlaySequence(SuccessSequence);
	}
	else if (FailSequence)
	{
		PlaySequence(FailSequence);
	}

	// 2.5초 후 UI 복구
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