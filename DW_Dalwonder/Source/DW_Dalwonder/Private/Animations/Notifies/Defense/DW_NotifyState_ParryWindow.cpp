#include "Animations/Notifies/Defense/DW_NotifyState_ParryWindow.h"
#include "Character/DW_CharacterBase.h"

void UDW_NotifyState_ParryWindow::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (ADW_CharacterBase* Char = Cast<ADW_CharacterBase>(MeshComp->GetOwner()))
	{
		Char->SetParrying(true);
	}
}

void UDW_NotifyState_ParryWindow::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (ADW_CharacterBase* Char = Cast<ADW_CharacterBase>(MeshComp->GetOwner()))
	{
		Char->SetParrying(false);
	}
}