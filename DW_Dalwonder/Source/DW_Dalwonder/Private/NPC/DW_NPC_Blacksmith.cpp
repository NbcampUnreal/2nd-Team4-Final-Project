#include "NPC/DW_NPC_Blacksmith.h"
#include "DW_GmBase.h"
#include "DW_SmiteUI.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "NeutralityNPC/ItemEnhanceManager.h"
#include "NeutralityNPC/ItemCraftManager.h"

ADW_NPC_Blacksmith::ADW_NPC_Blacksmith()
{
}

void ADW_NPC_Blacksmith::BeginPlay()
{
	Super::BeginPlay();
	
	if (HammerStaticMesh)
	{
		HammerComponent = NewObject<UStaticMeshComponent>(this, TEXT("HammerComponent"));
		HammerComponent->RegisterComponent();
		HammerComponent->SetStaticMesh(HammerStaticMesh);
		HammerComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r"));
	}
	
	PlayIdleAnimation();
}

void ADW_NPC_Blacksmith::HandleEnhancementAction()
{
	// 기존 부착 메시 제거
	if (TargetItemComponent)
	{
		TargetItemComponent->DestroyComponent();
		TargetItemComponent = nullptr;
	}

	// 새 메시 생성

	TargetItemComponent = NewObject<UStaticMeshComponent>(this, TEXT("ItemComponent"));
	TargetItemComponent->RegisterComponent();
	TargetItemComponent->SetStaticMesh(ItemStaticMesh);
	TargetItemComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_l"));
	
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

	FocusCameraOnNPC(Interactor);

	if (ADW_GmBase* GM = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
	{
		if (UUserWidget* Popup = GM->ShowPopupUI(SmiteWidgetClass))
		{
			if (UDW_SmiteUI* SmiteUI = Cast<UDW_SmiteUI>(Popup))
			{
				SmiteUI->RelatedNPC = this;
			}
		}
	}
}

void ADW_NPC_Blacksmith::HandleCraftAction()
{
	if (!CraftManager || !Inventory || !SelectedItem)
	{
		UE_LOG(LogTemp, Warning, TEXT("CraftManager 또는 Inventory 또는 SelectedItem 누락"));
		return;
	}

	const bool bSuccess = CraftManager->TryCraftItem(SelectedItem, Inventory, 1);
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("제작 성공: %s"), *SelectedItem->ItemCode);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("제작 실패"));
	}
	
	PlayEnhanceAnimation();
}

bool ADW_NPC_Blacksmith::HandleEnhanceLogic()
{
	// if (!EnhanceManager || !Inventory || !SelectedEnhanceItem)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("강화 데이터 누락"));
	// 	return false;
	// }

	const bool bSuccess = EnhanceManager->TryEnhanceItem(SelectedEnhanceItem, Inventory);
	if (bSuccess)
	{
		UE_LOG(LogTemp, Log, TEXT("강화 성공"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("강화 실패"));
	}

	return bSuccess;
}
