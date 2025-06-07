//Copyright with Twiluna Studio

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DW_GmBase.generated.h"

class UUserWidget;

UCLASS()
class DW_DALWONDER_API ADW_GmBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    ADW_GmBase();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "UI") // UI 전체 관리
    void SwitchUI(TSubclassOf<UUserWidget> NewWidgetClass);

    // 팝업 UI 관리
    UPROPERTY()
    TArray<UUserWidget*> PopupWidgets;

    UFUNCTION(BlueprintCallable, Category = "UI")
    UUserWidget* ShowPopupUI(TSubclassOf<UUserWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ClosePopupUI(UUserWidget* WidgetToRemove);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseLastPopupUI();

    //차례로 닫고 마지막 ESC닫기용
    UUserWidget* CloseLastPopupUI_AndReturn();

    // 열려있는 UI 반환
    int32 GetPopupWidgetCount() const { return PopupWidgets.Num(); }

	UFUNCTION(BlueprintCallable)
	void ShowResultUI(const FString& MessageText);

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> ResultWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> BossHUDWidgetClass;
	
	// UUserWidget* ActiveResultWidget;

protected:
    UPROPERTY()
    UUserWidget* CurrentWidget;
};
