#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DW_NotifyState_ParryWindow.generated.h"

UCLASS()
class DW_DALWONDER_API UDW_NotifyState_ParryWindow : public UAnimNotifyState
{
	GENERATED_BODY()

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
