#include "DW_Notify_SetVisibility.h"
#include "Character/DW_CharacterBase.h"

UDW_Notify_SetVisibility::UDW_Notify_SetVisibility()
{
}

void UDW_Notify_SetVisibility::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ADW_CharacterBase* PlayerCharacter = Cast<ADW_CharacterBase>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->SetVehicleVisibility(false);
	}
}
