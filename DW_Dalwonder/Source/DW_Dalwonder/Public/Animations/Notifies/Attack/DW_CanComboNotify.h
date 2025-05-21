#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DW_CanComboNotify.generated.h"

class ADW_CharacterBase;

UCLASS()
class DW_DALWONDER_API UDW_CanComboNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UDW_CanComboNotify();
	
	UFUNCTION()
	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	ADW_CharacterBase* PlayerCharacter;
};
