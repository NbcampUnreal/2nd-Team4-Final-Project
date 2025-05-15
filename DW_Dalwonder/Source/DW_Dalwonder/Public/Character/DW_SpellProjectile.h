#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DW_SpellProjectile.generated.h"

class UProjectileMovementComponent;
class UNiagaraSystem;
class UCapsuleComponent;

UCLASS()
class DW_DALWONDER_API ADW_SpellProjectile : public AActor
{
	GENERATED_BODY()

public:
	ADW_SpellProjectile();
	
	void FireInDirection(const FVector& Direction);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UNiagaraSystem* SpellNiagara;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UCapsuleComponent* ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USceneComponent* ProjectileTraceStartPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USceneComponent* ProjectileTraceEndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileSpeed = 2000.f;
};
