#include "DW_Hp_Test.h"
#include "Components/ProgressBar.h"
#include "DW_Hp_Test.h"

void UDW_Hp_Test::NativeConstruct()
{
    Super::NativeConstruct();

    // 체력 초기화
    CurrentHealth = MaxHealth;

    UpdateHealthBar();  // 초기 상태에서 체력 바 갱신
}

void UDW_Hp_Test::TakeDamage(float DamageAmount)
{
    // 체력 감소
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

    // 체력바 업데이트
    UpdateHealthBar();
}

void UDW_Hp_Test::UpdateHealthBar()
{
    if (HealthProgressBar)
    {
        float HealthPercent = CurrentHealth / MaxHealth;
        HealthProgressBar->SetPercent(HealthPercent);
    }
}