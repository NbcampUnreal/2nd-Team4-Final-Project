#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "DW_SmiteUI.generated.h"

UCLASS()
class DW_DALWONDER_API UDW_SmiteUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	// ��ư ���ε�
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Smite;

	// ������ �ڻ� (�������Ʈ���� �Ҵ�)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhance")
	ULevelSequence* SuccessSequence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enhance")
	ULevelSequence* FailSequence;

	// Ŭ�� �̺�Ʈ �Լ�
	UFUNCTION()
	void OnSmiteButtonClicked();

	// ������ ��� �Լ�
	void PlaySequence(ULevelSequence* Sequence);

	// UI ���� �Լ�
	UFUNCTION()
	void UnhideUI();

private:
	FTimerHandle UnhideUITimerHandle;
};
