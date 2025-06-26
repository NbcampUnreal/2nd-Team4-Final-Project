// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/DW_SkillTooltip.h"
#include "Components/TextBlock.h"

void UDW_SkillTooltip::SetSkillTooltipInfo(const FText& Name, const FText& Desc, int32 CurrentLevel, int32 MaxLevel)
{
	UE_LOG(LogTemp, Log, TEXT("Setting Skill Tooltip Info: %s, %s, Lv %d / %d"), *Name.ToString(), *Desc.ToString(), CurrentLevel, MaxLevel);
    if (SkillNameText) SkillNameText->SetText(Name);
    if (SkillDescText) SkillDescText->SetText(Desc);

    if (SkillLevelText)
    {
        SkillLevelText->SetText(FText::FromString(FString::Printf(TEXT("Lv %d / %d"), CurrentLevel, MaxLevel)));
    }
}
