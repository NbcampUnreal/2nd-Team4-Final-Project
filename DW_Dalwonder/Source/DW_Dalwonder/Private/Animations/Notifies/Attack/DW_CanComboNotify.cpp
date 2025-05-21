#include "Animations/Notifies/Attack/DW_CanComboNotify.h"
#include "Character/DW_CharacterBase.h"

UDW_CanComboNotify::UDW_CanComboNotify()
{
	PlayerCharacter = nullptr;
}

void UDW_CanComboNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	PlayerCharacter = Cast<ADW_CharacterBase>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->bCanCombo = true;
	}
}

void UDW_CanComboNotify::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->bCanCombo = false;
	}
}
