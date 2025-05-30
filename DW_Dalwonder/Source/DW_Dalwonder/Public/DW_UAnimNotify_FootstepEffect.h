#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DW_UAnimNotify_FootstepEffect.generated.h"


UCLASS()
class DW_DALWONDER_API UDW_UAnimNotify_FootstepEffect : public UAnimNotify
{
	GENERATED_BODY()

public:
	UDW_UAnimNotify_FootstepEffect();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Footstep")
	FName FootSocketName = "foot_l";

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
