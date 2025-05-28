#include "Character/DW_Warrior.h"

ADW_Warrior::ADW_Warrior()
{
	TArray WeaponSocketNames = { FName(TEXT("weapon_r1Socket")), FName(TEXT("weapon_rSocket")) };
	
	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));

	//@TODO : 무기 식별 코드에 따라 장착하는 소켓 다르게 설정
	if (true) //GetWeapon()->그냥 검 어쩌구 식별코드)
	{
		Weapon->SetupAttachment(GetMesh(), WeaponSocketNames[0]);
	}
	else if (false) // GetWeapon()->대검 어쩌구 식별코드)
	{
		Weapon->SetupAttachment(GetMesh(), WeaponSocketNames[1]);
	}
}

void ADW_Warrior::BeginPlay()
{
	Super::BeginPlay();

}

void ADW_Warrior::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}
