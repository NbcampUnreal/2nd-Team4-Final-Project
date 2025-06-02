#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "WeaponBase.generated.h"

USTRUCT(BlueprintType)
struct FWeaponEnhancementData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnhancementLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed;
};


UCLASS(Blueprintable)
class DW_DALWONDER_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

	// 무기 고유 ID (ex. Asword)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Base")
	FName WeaponID;

	// 현재 강화 단계
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Base")
	int32 EnhancementLevel;

	// 현재 스탯 (강화 반영)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Base")
	float AttackPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Base")
	float AttackSpeed;

	// 강화 수치가 들어 있는 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Base")
	UDataTable* EnhancementDataTable;

	// 무기의 루트 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Base")
	USceneComponent* GrabPoint;

	// 무기의 스태틱 메시 (외형)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Base")
	UStaticMeshComponent* StaticMesh;
	
	// 무기 여부 확인용
	UFUNCTION(BlueprintCallable, Category = "Weapon Base")
	virtual bool IsWeapon() const { return true; }

	// 강화 수행
	UFUNCTION(BlueprintCallable, Category = "Weapon Base")
	virtual void EnhanceWeapon();

	// 데이터 테이블에서 현재 강화 단계에 맞는 스탯을 적용
	UFUNCTION(BlueprintCallable, Category = "Weapon Base")
	virtual void UpdateStatsFromDataTable();
};