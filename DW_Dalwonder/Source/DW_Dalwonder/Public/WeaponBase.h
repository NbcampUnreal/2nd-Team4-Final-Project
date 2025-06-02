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

	// ���� ���� ID (ex. Asword)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Base")
	FName WeaponID;

	// ���� ��ȭ �ܰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Base")
	int32 EnhancementLevel;

	// ���� ���� (��ȭ �ݿ�)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Base")
	float AttackPower;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Base")
	float AttackSpeed;

	// ��ȭ ��ġ�� ��� �ִ� ������ ���̺�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Base")
	UDataTable* EnhancementDataTable;

	// ������ ��Ʈ ������Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Base")
	USceneComponent* GrabPoint;

	// ������ ����ƽ �޽� (����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Base")
	UStaticMeshComponent* StaticMesh;
	
	// ���� ���� Ȯ�ο�
	UFUNCTION(BlueprintCallable, Category = "Weapon Base")
	virtual bool IsWeapon() const { return true; }

	// ��ȭ ����
	UFUNCTION(BlueprintCallable, Category = "Weapon Base")
	virtual void EnhanceWeapon();

	// ������ ���̺��� ���� ��ȭ �ܰ迡 �´� ������ ����
	UFUNCTION(BlueprintCallable, Category = "Weapon Base")
	virtual void UpdateStatsFromDataTable();
};