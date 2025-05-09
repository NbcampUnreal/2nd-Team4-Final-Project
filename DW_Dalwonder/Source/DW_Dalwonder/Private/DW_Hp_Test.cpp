#include "DW_Hp_Test.h"
#include "Components/ProgressBar.h"
#include "DW_Hp_Test.h"

void UDW_Hp_Test::NativeConstruct()
{
    Super::NativeConstruct();

    // ü�� �ʱ�ȭ
    CurrentHealth = MaxHealth;

    UpdateHealthBar();  // �ʱ� ���¿��� ü�� �� ����
}

void UDW_Hp_Test::TakeDamage(float DamageAmount)
{
    // ü�� ����
    CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

    // ü�¹� ������Ʈ
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