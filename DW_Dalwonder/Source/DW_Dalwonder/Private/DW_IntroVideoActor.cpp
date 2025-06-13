#include "DW_IntroVideoActor.h"
#include "DW_IntroVideoWidget.h"
#include "MediaPlayer.h"
#include "MediaTexture.h"
#include "MediaSoundComponent.h"
#include "FileMediaSource.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"


ADW_IntroVideoActor::ADW_IntroVideoActor()
{
	PrimaryActorTick.bCanEverTick = false;
	MediaSoundComponent = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("MediaSound"));
	MediaSoundComponent->SetupAttachment(RootComponent);
}

void ADW_IntroVideoActor::BeginPlay()
{
	Super::BeginPlay();

	if (!MediaSource || !MediaTexture)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("MediaSource 또는 MediaTexture가 설정되지 않았습니다."));
#endif
		return;
	}

	// MediaPlayer 생성
	MediaPlayer = NewObject<UMediaPlayer>(this, UMediaPlayer::StaticClass());

	// MediaTexture에 MediaPlayer 연결
	MediaTexture->SetMediaPlayer(MediaPlayer);

	// 위젯 생성 및 표시
	if (IntroWidgetClass)
	{
		UDW_IntroVideoWidget* IntroWidget = CreateWidget<UDW_IntroVideoWidget>(GetWorld(), IntroWidgetClass);
		if (IntroWidget)
		{
			IntroWidget->AddToViewport();
			IntroWidget->SetMediaTexture(MediaTexture);
		}
	}

	MediaPlayer->OnEndReached.AddDynamic(this, &ADW_IntroVideoActor::OnMediaEndReached);

	MediaPlayer->OpenSource(MediaSource);

	MediaSoundComponent->SetMediaPlayer(MediaPlayer);

	MediaPlayer->OpenSource(MediaSource);
}

void ADW_IntroVideoActor::OnMediaEndReached()
{
	GetWorldTimerManager().SetTimer(LevelTransitionTimer,
		this, &ADW_IntroVideoActor::TransitionToNextLevel, DelayAfterVideo, false);
}

void ADW_IntroVideoActor::TransitionToNextLevel()
{
	UGameplayStatics::OpenLevel(this, LevelToLoad);
}
