#include "Animations/Notifies/Attack/DW_Notify_AttackEnd.h"

#include "Character/DW_CharacterBase.h"

void UDW_Notify_AttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ADW_CharacterBase* Char = Cast<ADW_CharacterBase>(MeshComp->GetOwner()))
	{
		Char->SetCombatState(ECharacterCombatState::Idle);
	}
}
