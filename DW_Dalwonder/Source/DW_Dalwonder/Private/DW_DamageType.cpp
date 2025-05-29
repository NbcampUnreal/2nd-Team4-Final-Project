#include "DW_DamageType.h"

UDW_DamageType::UDW_DamageType()
{
}

UDW_DamageType* UDW_DamageType::CreateDamageType(bool bCanKnockdown)
{
	UDW_DamageType* NewDamageType = NewObject<UDW_DamageType>();
	if (IsValid(NewDamageType))
	{
		NewDamageType->bCanKnockdown = bCanKnockdown;
	}

	return NewDamageType;
}
