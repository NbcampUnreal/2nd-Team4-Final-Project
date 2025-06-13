// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/PlayerInfoWidget.h"
#include "UI/Widget/CustomButtonWidget.h"
#include "DW_GmBase.h"
#include "Character/DW_CharacterBase.h"
#include "DW_AttributeComponent.h"
#include "Character/CharacterStatComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SceneCapture2D.h"

void UPlayerInfoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 캐릭터 참조
	if (ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		//무게 값 없음
		float Damage = 0;
		if (UDW_AttributeComponent* Stat = Player->AttributeComponent)
		{
			UpdatePlayerInfo(
				Stat->GetMaxHealth(), Stat->GetMaxStamina(),
				Damage, Stat->GetDefense(),
				Stat->GetMaxCarryWeight(), Stat->GetMoveSpeed(),
				Stat->GetHealthRegen(), Stat->GetStaminaRegen()
			);
		}
	}

	//뒤로가기 버튼
	if (BackButton)
		BackButton->OnCustomClicked.AddDynamic(this, &UPlayerInfoWidget::OnBackClicked);
}

void UPlayerInfoWidget::UpdatePlayerInfo(float MaxHP, float MaxStamina, float Damage, float Defense, float Weight, float MoveSpeed, float HPRegen, float StaminaRegen)
{
	if (HPText)        HPText->SetText(FText::FromString(FString::Printf(TEXT("HP : %.0f"), MaxHP)));
	if (SteminaText)   SteminaText->SetText(FText::FromString(FString::Printf(TEXT("Stamina : %.0f"), MaxStamina)));
	if (DamageText)    DamageText->SetText(FText::FromString(FString::Printf(TEXT("Damage : %.0f"), Damage)));
	if (DefenceText)   DefenceText->SetText(FText::FromString(FString::Printf(TEXT("Defense : %.0f"), Defense)));
	if (WeightText)    WeightText->SetText(FText::FromString(FString::Printf(TEXT("Weight : %.0f"), Weight)));
	if (MoveSpeedText) MoveSpeedText->SetText(FText::FromString(FString::Printf(TEXT("Move Speed : %.0f"), MoveSpeed)));
	if (HPRegenText)   HPRegenText->SetText(FText::FromString(FString::Printf(TEXT("HP Regen : %.1f"), HPRegen)));
	if (StRegenText)   StRegenText->SetText(FText::FromString(FString::Printf(TEXT("Stamina Regen : %.1f"), StaminaRegen)));
}

void UPlayerInfoWidget::OnBackClicked()
{
	if (ADW_GmBase* GameMode = Cast<ADW_GmBase>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->CloseLastPopupUI();  // 현재 위젯 닫기
	}
}
