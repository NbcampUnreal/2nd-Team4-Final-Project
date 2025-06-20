#include "NeutralityNPC/DW_BlacksmithNpc.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"

ADW_BlacksmithNpc::ADW_BlacksmithNpc()
{
}

void ADW_BlacksmithNpc::BeginPlay()
{
	Super::BeginPlay();
	PlayIdleAnimation();
}

void ADW_BlacksmithNpc::HandleEnhancementAction()
{
	PlayEnhanceAnimation();
}

void ADW_BlacksmithNpc::PlayEnhanceAnimation()
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

void ADW_BlacksmithNpc::PlayIdleAnimation()
{
	USkeletalMeshComponent* MeshComp = GetNpcMesh();
	if (!MeshComp || !IdleMontage) return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(IdleMontage))
	{
		AnimInstance->Montage_Play(IdleMontage);
	}
}
