#include "DW_Mage.h"

ADW_Mage::ADW_Mage()
{
	FName WeaponSocketName(TEXT("weapon_l"));
	
	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), WeaponSocketName);
}

void ADW_Mage::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADW_Mage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

