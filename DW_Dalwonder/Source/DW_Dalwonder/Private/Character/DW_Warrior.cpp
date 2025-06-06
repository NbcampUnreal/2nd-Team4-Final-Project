#include "Character/DW_Warrior.h"

ADW_Warrior::ADW_Warrior()
{
	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());
}

void ADW_Warrior::BeginPlay()
{
	Super::BeginPlay();

}

void ADW_Warrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ADW_Warrior::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FName WeaponSocketName = FName(TEXT("weapon_rSocket"));
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
}
