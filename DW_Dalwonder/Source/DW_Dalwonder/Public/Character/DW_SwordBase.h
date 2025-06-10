#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Item/WorldItemActor.h"
#include "DW_SwordBase.generated.h"

UCLASS()
class DW_DALWONDER_API ADW_SwordBase : public AActor
{
	GENERATED_BODY()

public:
	ADW_SwordBase();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	USceneComponent* SwordTraceStartPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sword")
	USceneComponent* SwordTraceEndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UItemBase* ItemBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace")
	float CapsuleTraceRadius = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace")
	float CapsuleTraceHalfHeight = 30.f;
};
