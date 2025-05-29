#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DW_SwordAttackNotify.generated.h"

class ADW_Warrior;
class ADW_SwordBase;

UCLASS()
class DW_DALWONDER_API UDW_SwordAttackNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UDW_SwordAttackNotify();
	
	UFUNCTION()
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	UFUNCTION()
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackDamage;

	UPROPERTY()
	ADW_Warrior* PlayerCharacter;
	
	UPROPERTY()
	ADW_SwordBase* CharacterWeapon;

	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> IgnoredActors;
	
private:
	FVector PrevTraceStart;
	FVector PrevTraceEnd;
	bool bHasPrevTrace = false;
};
