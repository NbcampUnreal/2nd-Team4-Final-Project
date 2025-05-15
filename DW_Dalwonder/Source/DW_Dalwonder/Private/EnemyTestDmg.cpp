#include "EnemyTestDmg.h"
#include "UI/DW_DamageText.h"
#include "Kismet/GameplayStatics.h"

AEnemyTestDmg::AEnemyTestDmg()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentHealth = MaxHealth;
}

void AEnemyTestDmg::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;
}

//float AEnemyTestDmg::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
//    AController* EventInstigator, AActor* DamageCauser)
//{
//    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//
//    if (ActualDamage > 0.f)
//    {
//        CurrentHealth -= ActualDamage;
//        CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);
//
//        // 데미지 텍스트 액터 생성
//        if (DamageTextActorClass)
//        {
//            FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 100.f);
//
//            FActorSpawnParameters SpawnParams;
//            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//            ADW_DamageText* DamageTextActor = GetWorld()->SpawnActor<ADW_DamageText>(
//                DamageTextActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
//
//            if (DamageTextActor)
//            {
//                DamageTextActor->SetDamage(ActualDamage);
//            }
//        }
//    }
//
//    return ActualDamage;
//}
