#include "DW_Mage.h"
#include "DW_SpellProjectile.h"
#include "GameFramework/SpringArmComponent.h"

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

void ADW_Mage::SpawnProjectile(TSubclassOf<ADW_SpellProjectile>& Spell)
{
	if (IsValid(Spell) == true)
	{
		MuzzleOffset.Set(100.f, 0.f, 0.f);
		FVector MuzzleLocation = GetActorLocation() + SpringArm->GetForwardVector() * FTransform(SpringArm->GetComponentRotation()).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = GetActorRotation();
		MuzzleRotation.Pitch += 10.f;

		if (GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			ADW_SpellProjectile* Projectile = GetWorld()->SpawnActor<ADW_SpellProjectile>(Spell, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (IsValid(Projectile) == true)
			{
				const FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}

void ADW_Mage::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

