#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_Sword.generated.h"

class UCapsuleComponent;

UCLASS()
class DW_DALWONDER_API ADW_Sword : public AActor
{
	GENERATED_BODY()

public:
	ADW_Sword();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSwordBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	UPROPERTY(VIsibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	UCapsuleComponent* WeaponCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	USceneComponent* SwordTraceStartPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	USceneComponent* SwordTraceEndPoint;
};
