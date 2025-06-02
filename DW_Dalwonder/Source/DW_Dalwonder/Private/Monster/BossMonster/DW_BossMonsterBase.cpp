#include "Monster/BossMonster/DW_BossMonsterBase.h"

#include "AIController.h"
#include "Character/DW_CharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AudioComponent.h"

ADW_BossMonsterBase::ADW_BossMonsterBase(): CurrentPhase(1), BGM(nullptr)
{
	PrimaryActorTick.bCanEverTick = false;

	BGMSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGM"));
	BGMSoundComponent->SetupAttachment(RootComponent);
	BGMSoundComponent->bAutoActivate = false;

	Tags.Add(TEXT("BossMonster"));
}

void ADW_BossMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (AAIController* Ctr = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
		{
			BBC->SetValueAsObject(FName("TargetActor"), PlayerCharacter);
		}
	}
}

int32 ADW_BossMonsterBase::GetCurrentPhase()
{
	return CurrentPhase;
}

void ADW_BossMonsterBase::SetCurrentPhase(int32 NewPhase)
{
	CurrentPhase = NewPhase;

	if (AAIController* Ctr = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBC = Ctr->GetBlackboardComponent())
		{
			BBC->SetValueAsInt(FName("CurrentPhase"), NewPhase);
		}
	}
}

void ADW_BossMonsterBase::SetBGM(USoundBase* NewBGM)
{
	//BGM 변경로직
}
