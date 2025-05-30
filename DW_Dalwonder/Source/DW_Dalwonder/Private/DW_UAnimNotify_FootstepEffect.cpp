#include "DW_UAnimNotify_FootstepEffect.h"
#include "Character/DW_CharacterBase.h"


UDW_UAnimNotify_FootstepEffect::UDW_UAnimNotify_FootstepEffect()
{
	
}


void UDW_UAnimNotify_FootstepEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ADW_CharacterBase* Character = Cast<ADW_CharacterBase>(MeshComp->GetOwner()))
	{
		Character->SpawnFootstepEffect(FootSocketName);
	}
}