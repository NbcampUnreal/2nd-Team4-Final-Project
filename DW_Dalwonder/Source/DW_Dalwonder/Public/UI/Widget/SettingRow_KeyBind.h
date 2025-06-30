#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputCoreTypes.h"
#include "SettingRow_KeyBind.generated.h"

class UHorizontalBox;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnKeyAssigned, FName, ActionName, FKey, NewKey);

UCLASS()
class DW_DALWONDER_API USettingRow_KeyBind : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyBind", meta = (ExposeOnSpawn))
	FText LabelText;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	/** 외부에서 이 액션이 뭔지 지정 (Jump, Attack 등) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "KeyBind")
	FName ActionName;

	/** 현재 할당된 키 */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "KeyBind")
	FKey AssignedKey;

	/** 키 변경 완료 시 알림 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "KeyBind")
	FOnKeyAssigned OnKeyAssigned;

	/** 입력 대기 시작 (버튼 클릭 시 호출됨) */
	UFUNCTION(BlueprintCallable)
	void StartListeningForKey();

	UFUNCTION()
	void RefreshKeyDisplay();

	UFUNCTION()
	void HandleResetToDefault();

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Reset;

	UFUNCTION()
	void OnResetButtonClicked();

	/** 초기화 함수 (ActionName, AssignedKey, LabelText) */
	UFUNCTION(BlueprintCallable)
	void Init(FName InActionName, FKey InAssignedKey, FText InLabelText);

	UPROPERTY(meta = (BindWidget))
	UButton* Button_AssignKey;

protected:
	/** 입력 대기 중 여부 */
	bool bIsListeningForInput = false;

	/** UI 요소 접근용 (Expose on BP에서 바인딩) */
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_KeyName;
};
