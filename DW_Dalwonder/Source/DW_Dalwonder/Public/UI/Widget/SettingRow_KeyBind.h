// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "SettingRow_KeyBind.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKeyAssigned, FName, ActionName, FKey, NewKey);

UCLASS()
class DW_DALWONDER_API USettingRow_KeyBind : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	/** 외부에서 이 액션이 뭔지 지정 (Jump, Attack 등) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyBind")
	FName ActionName;

	/** 현재 할당된 키 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyBind")
	FKey AssignedKey;

	/** 키 변경 완료 시 알림 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "KeyBind")
	FOnKeyAssigned OnKeyAssigned;

	/** 입력 대기 시작 (버튼 클릭 시 호출됨) */
	UFUNCTION(BlueprintCallable)
	void StartListeningForKey();

protected:
	/** 입력 대기 중 여부 */
	bool bIsListeningForInput = false;

	/** UI 요소 접근용 (Expose on BP에서 바인딩) */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_KeyName;
};
