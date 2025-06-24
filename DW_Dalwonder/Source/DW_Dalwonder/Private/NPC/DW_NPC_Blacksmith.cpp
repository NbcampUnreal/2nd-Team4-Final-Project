#include "NPC/DW_NPC_Blacksmith.h"

#include "DW_GmBase.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

ADW_NPC_Blacksmith::ADW_NPC_Blacksmith()
{
}

void ADW_NPC_Blacksmith::BeginPlay()
{
	Super::BeginPlay();
	PlayIdleAnimation();
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

void ADW_NPC_Blacksmith::Interact_Implementation(AActor* Interactor)
{
	Super::Interact_Implementation(Interactor);
	UE_LOG(LogTemp, Warning, TEXT("Blacksmith Interacted"));
	// 1. 카메라 포커스
	FocusCameraOnNPC(Interactor);

	// 2. UI 보여주기
	if (!SmiteWidgetClass) return;

	if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
	{
		GM->ShowPopupUI(SmiteWidgetClass);
	}
}
