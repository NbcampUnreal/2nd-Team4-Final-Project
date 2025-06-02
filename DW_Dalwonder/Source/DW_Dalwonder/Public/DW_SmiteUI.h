#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "DW_SmiteUI.generated.h"

class UButton;

UCLASS()
class DW_DALWONDER_API UDW_SmiteUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Smite;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhance")
	ULevelSequence* SuccessSequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhance")
	ULevelSequence* FailSequence;

	UFUNCTION()
	void OnSmiteButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Exit;

	void PlaySequence(ULevelSequence* Sequence);

	UFUNCTION()
	void UnhideUI();

private:
	FTimerHandle UnhideUITimerHandle;
};
