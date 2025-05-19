// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/SavedGameWidget.h"

void USavedGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("USavedGameWidget::NativeConstruct - 위젯 생성됨"));
}
