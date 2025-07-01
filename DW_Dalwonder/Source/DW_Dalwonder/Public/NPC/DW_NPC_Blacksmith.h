#pragma once

#include "CoreMinimal.h"
#include "DW_NpcBase.h"
#include "DW_NPC_Blacksmith.generated.h"

class UAnimMontage;
class UItemBase;
class UItemCraftManager;
class UInventoryComponent;
class UItemEnhanceManager;

UCLASS()
class DW_DALWONDER_API ADW_NPC_Blacksmith : public ADW_NpcBase
{
	GENERATED_BODY()
public:
	ADW_NPC_Blacksmith();
	void BeginPlay();
	virtual void Interact_Implementation(AActor* Interactor) override;

	UFUNCTION(BlueprintCallable, Category = "Blacksmith")
	void HandleCraftAction();
	bool HandleEnhanceLogic();
	UFUNCTION(BlueprintCallable, Category = "Blacksmith")
	void HandleEnhancementAction();
	

	/** 강화 애니메이션 재생 */
	UFUNCTION(BlueprintCallable, Category = "Blacksmith")
	void PlayEnhanceAnimation();
	UFUNCTION(BlueprintCallable, Category = "Blacksmith")
	void PlayIdleAnimation();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhance")
	UItemEnhanceManager* EnhanceManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhance")
	UItemBase* SelectedEnhanceItem;

	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
	UItemCraftManager* CraftManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
	UInventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Craft")
	UItemBase* SelectedItem;
	
	UPROPERTY()
	UStaticMeshComponent* HammerComponent;

	UPROPERTY()
	UStaticMeshComponent* TargetItemComponent;

	// 오른손용 망치 메시
	UPROPERTY(EditDefaultsOnly, Category = "Enhancement")
	UStaticMesh* HammerStaticMesh;

	// 왼손용 강화 대상 아이템 메시
	UPROPERTY(EditDefaultsOnly, Category = "Enhancement")
	UStaticMesh* ItemStaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blacksmith|Animation")
	UAnimMontage* EnhanceMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blacksmith|Animation")
	UAnimMontage* IdleMontage;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> SmiteWidgetClass;

};
