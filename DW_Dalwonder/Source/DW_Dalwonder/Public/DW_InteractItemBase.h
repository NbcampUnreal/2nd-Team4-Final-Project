#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_InteractInterface.h"
#include "Components/WidgetComponent.h"
#include "DW_InteractItemBase.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_InteractItemBase : public AActor, public IDW_InteractInterface
{
    GENERATED_BODY()

public:
    ADW_InteractItemBase();

protected:
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    // UWidgetComponent�� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* InteractionWidget;

    // UUserWidget Ŭ������ �����ϴ� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> InteractionWidgetClass;

public:
    virtual void Interact_Implementation(AActor* Interactor) override;
    virtual void ShowInteractionWidget_Implementation() override;
    virtual void HideInteractionWidget_Implementation() override;
};
