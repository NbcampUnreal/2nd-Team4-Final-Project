#include "Character/DW_Warrior.h"

ADW_Warrior::ADW_Warrior()
{
	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	
	FName WeaponSocketName = "weapon_r";
	if (WeaponType == 0)
	{
		WeaponSocketName = FName(TEXT("weapon_rSocket_Sword"));
	}
	if (WeaponType == 1)
	{
		WeaponSocketName = FName(TEXT("weapon_rSocket_Greatsword"));
	}
	
	Weapon->SetupAttachment(GetMesh(), WeaponSocketName);
}

void ADW_Warrior::BeginPlay()
{
	Super::BeginPlay();

}

void ADW_Warrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ADW_Warrior::SetWeaponType(int32 NewWeaponType)
{
	Super::SetWeaponType(NewWeaponType);
	
	FName WeaponSocketName = "weapon_r";

	if (WeaponType == 0)
	{
		WeaponSocketName = FName(TEXT("weapon_rSocket_Sword"));
	}
	if (WeaponType == 1)
	{
		WeaponSocketName = FName(TEXT("weapon_rSocket_Greatsword"));
	}

	Weapon->SetupAttachment(GetMesh(), WeaponSocketName);
}
