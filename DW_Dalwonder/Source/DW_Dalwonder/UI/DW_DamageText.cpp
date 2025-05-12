#include "UI/DW_DamageText.h"
#include "Components/WidgetComponent.h"
#include "DW_Damageimg.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADW_DamageText::ADW_DamageText()
{
    PrimaryActorTick.bCanEverTick = false;

    WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
    RootComponent = WidgetComponent;

    WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen); // ¶Ç´Â World
    WidgetComponent->SetDrawSize(FVector2D(200, 50));
    WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void ADW_DamageText::BeginPlay()
{
	Super::BeginPlay();
	
    SetLifeSpan(1.5f);
}

void ADW_DamageText::SetDamage(float Damage)
{
    if (UUserWidget* Widget = WidgetComponent->GetUserWidgetObject())
    {
        if (UDW_Damageimg* DamageWidget = Cast<UDW_Damageimg>(Widget))
        {
            DamageWidget->UpdateDamageani(Damage);
        }
    }
}

