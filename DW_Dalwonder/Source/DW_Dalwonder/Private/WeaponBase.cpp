#include "WeaponBase.h"
#include "Engine/DataTable.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetSystemLibrary.h"

AWeaponBase::AWeaponBase()
{
	AttackPower = 10.0f;
	AttackSpeed = 1.0f;

	GrabPoint = CreateDefaultSubobject<USceneComponent>(TEXT("GrabPoint"));
	SetRootComponent(GrabPoint);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(GrabPoint);
}

void AWeaponBase::EnhanceWeapon()
{
	EnhancementLevel++;
#if WITH_EDITOR
	UE_LOG(LogTemp, Warning, TEXT("무기가 강화되었습니다! 현재 강화 레벨: +%d"), EnhancementLevel);
#endif
	UpdateStatsFromDataTable();
}

void AWeaponBase::UpdateStatsFromDataTable()
{
	if (!EnhancementDataTable)
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Error, TEXT("EnhancementDataTable is not set on %s."), *GetName());
#endif
		return;
	}

	// RowName은 WeaponID_강화단계 형식 ("Asword_1")
	FString RowNameStr = FString::Printf(TEXT("%s_%d"), *WeaponID.ToString(), EnhancementLevel);
	FName RowName(*RowNameStr);

	const FWeaponEnhancementData* Row = EnhancementDataTable->FindRow<FWeaponEnhancementData>(RowName, TEXT("Weapon Enhancement Lookup"));

	if (Row)
	{
		AttackPower = Row->AttackPower;
		AttackSpeed = Row->AttackSpeed;

#if WITH_EDITOR
		UE_LOG(LogTemp, Log, TEXT("%s 강화 적용됨: 공격력 %.1f, 공격속도 %.2f"), *WeaponID.ToString(), AttackPower, AttackSpeed);
#endif
	}
	else
	{
#if WITH_EDITOR
		UE_LOG(LogTemp, Warning, TEXT("강화 데이터 없음: %s (레벨 %d)"), *WeaponID.ToString(), EnhancementLevel);
#endif
	}
}