#include "DW_Warrior.h"
#include "DW_Sword.h"

ADW_Warrior::ADW_Warrior()
{
	FName WeaponSocketName(TEXT("weapon_r"));
	
	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), WeaponSocketName);
}

void ADW_Warrior::BeginPlay()
{
	Super::BeginPlay();

	FName WeaponSocketName(TEXT("weapon_r"));

	UStaticMeshComponent* CurrentWeapon = GetWorld()->SpawnActor<UStaticMeshComponent>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (IsValid(CurrentWeapon) == true)
	{
		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachRules, WeaponSocketName);
	}
}

void ADW_Warrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}
