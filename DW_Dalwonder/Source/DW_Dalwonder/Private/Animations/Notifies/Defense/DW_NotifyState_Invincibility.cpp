#include "Animations/Notifies/Defense/DW_NotifyState_Invincibility.h"
#include "Character/DW_CharacterBase.h"

void UDW_NotifyState_Invincibility::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (ADW_CharacterBase* Char = Cast<ADW_CharacterBase>(MeshComp->GetOwner()))
	{
		Char->SetInvincible(true);
	}
}

void UDW_NotifyState_Invincibility::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ADW_CharacterBase* Char = Cast<ADW_CharacterBase>(MeshComp->GetOwner()))
	{
		Char->SetInvincible(false);
	}
}