#include "Animations/Notifies/Attack/DW_Notify_AttackStart.h"
#include "Character/DW_CharacterBase.h"

void UDW_Notify_AttackStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (ADW_CharacterBase* Char = Cast<ADW_CharacterBase>(MeshComp->GetOwner()))
	{
		Char->SetCombatState(ECharacterCombatState::Attacking);
	}
}