#include "NPC/DW_NPC_Repairman.h"
#include "Animation/AnimInstance.h"

void ADW_NPC_Repairman::BeginPlay()
{
	Super::BeginPlay();
	PlayEnhanceAnimation();
}

void ADW_NPC_Repairman::PlayEnhanceAnimation()
{
	USkeletalMeshComponent* MeshComp = FindComponentByClass<USkeletalMeshComponent>();
	if (!MeshComp || !EnhanceMontage) return;

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (!AnimInstance) return;

	AnimInstance->Montage_Play(EnhanceMontage);
}
