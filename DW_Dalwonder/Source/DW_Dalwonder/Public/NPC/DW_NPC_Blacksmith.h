#pragma once

#include "CoreMinimal.h"
#include "DW_NpcBase.h"
#include "DW_NPC_Blacksmith.generated.h"

class UAnimMontage;

UCLASS()
class DW_DALWONDER_API ADW_NPC_Blacksmith : public ADW_NpcBase
{
	GENERATED_BODY()
public:
	ADW_NPC_Blacksmith();
	void BeginPlay();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blacksmith|Animation")
	UAnimMontage* EnhanceMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blacksmith|Animation")
	UAnimMontage* IdleMontage;

public:
	/** 강화 실행 시 호출될 함수 (UI 버튼에서 호출) */
	UFUNCTION(BlueprintCallable, Category = "Blacksmith")
	void HandleEnhancementAction();

protected:
	/** 강화 애니메이션 재생 */
	void PlayEnhanceAnimation();
	void PlayIdleAnimation();
};
