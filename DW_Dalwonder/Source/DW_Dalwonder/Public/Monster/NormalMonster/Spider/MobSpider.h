// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/NormalMonster/DW_NormalMonsterBase.h"
#include "MobSpider.generated.h"

/**
 * 
 */
UCLASS()
class DW_DALWONDER_API AMobSpider : public ADW_NormalMonsterBase
{
	GENERATED_BODY()
	
public:
	AMobSpider();

	void DeadLogic() override;
};
