#include "Character/DW_Warrior.h"

ADW_Warrior::ADW_Warrior()
{
	FName WeaponSocketName(TEXT("weapon_r1Socket"));
	
	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
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
