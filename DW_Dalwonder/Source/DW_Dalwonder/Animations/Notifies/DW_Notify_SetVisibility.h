#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DW_Notify_SetVisibility.generated.h"

UCLASS()
class DW_DALWONDER_API UDW_Notify_SetVisibility : public UAnimNotify
{
	GENERATED_BODY()

public:
	UDW_Notify_SetVisibility();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
