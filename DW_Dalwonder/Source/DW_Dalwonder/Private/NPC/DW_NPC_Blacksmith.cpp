#include "NPC/DW_NPC_Blacksmith.h"
#include "Animation/AnimInstance.h"

ADW_NPC_Blacksmith::ADW_NPC_Blacksmith()
{
}

void ADW_NPC_Blacksmith::BeginPlay()
{
	Super::BeginPlay();
	// PlayIdleAnimation();
	HandleEnhancementAction();
}

void ADW_NPC_Blacksmith::HandleEnhancementAction()
{
	PlayEnhanceAnimation();
}

void ADW_NPC_Blacksmith::PlayEnhanceAnimation()
{
	USkeletalMeshComponent* MeshComp = FindComponentByClass<USkeletalMeshComponent>();
	if (!MeshComp || !EnhanceMontage) return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;

	FOnMontageEnded MontageEndDelegate;
	MontageEndDelegate.BindLambda([this](UAnimMontage* Montage, bool bInterrupted)
	{
		if (!bInterrupted)
		{
			PlayIdleAnimation();
		}
	});

	AnimInstance->Montage_Play(EnhanceMontage);
	AnimInstance->Montage_SetEndDelegate(MontageEndDelegate, EnhanceMontage);
}

void ADW_NPC_Blacksmith::PlayIdleAnimation()
{
	USkeletalMeshComponent* MeshComp = GetNpcMesh();
	if (!MeshComp || !IdleMontage) return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(IdleMontage))
	{
		AnimInstance->Montage_Play(IdleMontage);
	}
}
