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

    UFUNCTION(BlueprintCallable, Category = "UI") //UI교체 로직
    void SwitchUI(TSubclassOf<UUserWidget> NewWidgetClass);

    //팝업 UI 로직
    UPROPERTY()
    TArray<UUserWidget*> PopupWidgets;

    UFUNCTION(BlueprintCallable, Category = "UI")
    UUserWidget* ShowPopupUI(TSubclassOf<UUserWidget> WidgetClass);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ClosePopupUI(UUserWidget* WidgetToRemove);

    UFUNCTION(BlueprintCallable, Category = "UI")
    void CloseLastPopupUI();

protected:
    UPROPERTY()
    UUserWidget* CurrentWidget;
};
