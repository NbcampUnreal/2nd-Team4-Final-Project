// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Component/Struct/DialogueLine.h"
#include "UI/Component/Struct/QuestData.h"
#include "DialogueWidget.generated.h"

class UTextBlock;
class UButton;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    /** 텍스트 세팅 함수 */
    UFUNCTION(BlueprintCallable)
    void SetDialogueText(const TArray<FDialogueLine>& InDialogueLines);

    /** 대화 상대 이름 업데이트 함수 */
    UFUNCTION(BlueprintCallable)
    void SetSpeakerName(const FText& InName);

    /** 대화 넘기기 함수 */
    UFUNCTION(BlueprintCallable)
    void ShowNextDialogueLine();

    /** 퀘스트 호출 함수 */
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void InitQuest(FName InQuestID);

    UFUNCTION(BlueprintCallable)
    void OnNextButtonClicked();

    UFUNCTION(BlueprintCallable)
    void OnAcceptQuestClicked();

protected:
    /** 위젯 구성요소 */
    /** 바인딩된 텍스트 블록 */
    UPROPERTY(meta = (BindWidget))
    UTextBlock* DialogueText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuestQuestionText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* SpeakerNameText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* NexiDialogueText;

    UPROPERTY(meta = (BindWidget))
    UButton* NextDialogueButton;

    //퀘스트 수락버튼
    UPROPERTY(meta = (BindWidget))
    UButton* AcceptQuestButton;
    
protected:
    /** 변수 모음 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDialogueLine> DialogueLines;

    int32 CurrentDialogueIndex = 0;
    FQuestData CachedQuestData;

    // 수락할 퀘스트의 ID
    UPROPERTY(BlueprintReadOnly, Category = "Quest")
    FName QuestID;

    // 퀘스트가 포함된 대화인지 여부
    UPROPERTY(BlueprintReadOnly, Category = "Quest")
    bool bHasQuest = false;
};
