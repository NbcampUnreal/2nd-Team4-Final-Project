#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_IntroVideoActor.generated.h"

class UMediaSource;
class UMediaTexture;
class UMediaPlayer;
class UDW_IntroVideoWidget;

UCLASS()
class DW_DALWONDER_API ADW_IntroVideoActor : public AActor
{
	GENERATED_BODY()

public:
	ADW_IntroVideoActor();

protected:
	virtual void BeginPlay() override;

	/** ───────────── Delegates ───────────── */
	// 반드시 UFUNCTION!  파라미터 없어야 합니다.
	UFUNCTION()
	void OnMediaEndReached();

	/** ───────────── Helpers ───────────── */
	void TransitionToNextLevel();

	/** ───────────── Editable Props ───────────── */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Video")
	UMediaSource* MediaSource = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Video")
	UMediaTexture* MediaTexture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UDW_IntroVideoWidget> IntroWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Level")
	FName LevelToLoad = NAME_None;

	UPROPERTY(EditAnywhere, Category = "Timing")
	float DelayAfterVideo = 1.0f;

private:
	/** ───────────── Internals ───────────── */
	UPROPERTY()              // GC 보호
		UMediaPlayer* MediaPlayer = nullptr;

	FTimerHandle LevelTransitionTimer;
};
