// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Component/Struct/QuestData.h"
#include "QuestListWidget.generated.h"

class UExpandableArea;
class UVerticalBox;
class UQuestManagerComponent;
class UQuestEntryWidget;
class UQuestDetailPanel;
class UObjectiveEntryWidget;

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API UQuestListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;

    /** 항목 클릭 시 상세 표시 */
    UFUNCTION()
    void OnQuestSelected(const FQuestData& QuestData);

protected:
    /** 진행 중인 퀘스트 영역 */
    UPROPERTY(meta = (BindWidget))
    UExpandableArea* InProgressArea;

    /** 완료한 퀘스트 영역 */
    UPROPERTY(meta = (BindWidget))
    UExpandableArea* CompletedArea;

    /** 진행 중 퀘스트 리스트용 박스 */
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* InProgressList;

    /** 완료 퀘스트 리스트용 박스 */
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* CompletedList;

    /** 우측 퀘스트 상세 패널 */
    UPROPERTY(meta = (BindWidget))
    UQuestDetailPanel* QuestDetailPanel;

    /** 퀘스트 엔트리 위젯 클래스 */
    UPROPERTY(EditDefaultsOnly, Category = "Quest")
    TSubclassOf<UQuestEntryWidget> QuestEntryClass;

    /** 퀘스트 목표 진행도 위젯 클래스 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
    TSubclassOf<UObjectiveEntryWidget> ObjectiveEntryWidgetClass;

    /** 퀘스트 매니저 참조 */
    UPROPERTY()
    UQuestManagerComponent* QuestManager;

    /** 퀘스트 목록 갱신 */
    void RefreshQuestList();
};
