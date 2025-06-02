#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DW_DamageType.generated.h"

// 대미지 타입 생성은 UDW_DamageType::CreateDamageType(bool bCanKnockdown) 으로 생성
UCLASS()
class DW_DALWONDER_API UDW_DamageType : public UDamageType
{
	GENERATED_BODY()

public:
	// 공격이 Knockdown 시킬 수 있는 지 bCanKnockdown 으로 판별
	static UDW_DamageType* CreateDamageType(bool bCanKnockdown);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageType")
	bool bCanKnockdown = false;

private:
	UDW_DamageType();
};
