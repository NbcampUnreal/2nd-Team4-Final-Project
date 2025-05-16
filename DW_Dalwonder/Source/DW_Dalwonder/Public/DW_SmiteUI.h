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
	// 버튼 바인딩
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Smite;

	// 시퀀서 자산 (블루프린트에서 할당)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhance")
	ULevelSequence* SuccessSequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhance")
	ULevelSequence* FailSequence;

	// 클릭 이벤트 함수
	UFUNCTION()
	void OnSmiteButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* Button_Exit;

	// 시퀀서 재생 함수
	void PlaySequence(ULevelSequence* Sequence);

	// UI 복원 함수
	UFUNCTION()
	void UnhideUI();

private:
	FTimerHandle UnhideUITimerHandle;
};
