// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Component/Struct/QuestData.h"
#include "QuestDetailPanel.generated.h"

class UTextBlock;
class UScrollBox;
class UObjectiveEntryWidget;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UQuestDetailPanel : public UUserWidget
{
	GENERATED_BODY()

public:
    /** 퀘스트 정보를 패널에 표시 */
    UFUNCTION(BlueprintCallable)
    void SetQuestInfo(const FQuestData& QuestData);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuestTitleText;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* QuestDescriptionText;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* ObjectiveList;

    /** 목표 항목을 생성할 위젯 클래스 */
    UPROPERTY(EditDefaultsOnly, Category = "Quest")
    TSubclassOf<UObjectiveEntryWidget> ObjectiveEntryClass;
	
};
