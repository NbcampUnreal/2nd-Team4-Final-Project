#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyTestDmg.generated.h"

class ADW_DamageText;

UCLASS()
class DW_DALWONDER_API AEnemyTestDmg : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyTestDmg();

    /*virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
        class AController* EventInstigator, AActor* DamageCauser) override;*/

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    float CurrentHealth;

    // ������ ������ �ؽ�Ʈ ���� �������Ʈ Ŭ����
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<ADW_DamageText> DamageTextActorClass;
};
