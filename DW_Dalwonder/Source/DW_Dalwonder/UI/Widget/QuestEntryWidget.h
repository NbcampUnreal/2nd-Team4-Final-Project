// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Component/Struct/QuestData.h"
#include "QuestEntryWidget.generated.h"

class UTextBlock;
class UButton;
class UQuestListWidget;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UQuestEntryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    void InitWithData(const FQuestData& InData, UQuestListWidget* ParentWidget);

protected:
    virtual void NativeConstruct() override;

    UFUNCTION()
    void HandleClick();

    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuestTitleText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuestSummaryText;

    UPROPERTY(meta = (BindWidget))
    UButton* ClickableButton;

    /** 퀘스트 정보 저장 */
    FQuestData StoredQuestData;

    /** 부모 위젯 저장 */
    UPROPERTY()
    UQuestListWidget* OwnerListWidget;

};
