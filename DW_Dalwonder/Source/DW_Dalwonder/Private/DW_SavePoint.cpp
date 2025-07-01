// Fill out your copyright notice in the Description page of Project Settings.


#include "DW_SavePoint.h"
#include "Kismet/GameplayStatics.h"
#include "DW_GameInstance.h"
#include "Sound/SoundBase.h"

ADW_SavePoint::ADW_SavePoint()
{
}

void ADW_SavePoint::Interact_Implementation(AActor* Interactor)
{
    Super::Interact_Implementation(Interactor);

    ExecuteSave();

    // 사운드 있으면 쓰기
    if (SaveSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SaveSound, GetActorLocation());
    }
}

void ADW_SavePoint::ExecuteSave()
{
    if (UGameInstance* GI = GetGameInstance())
    {
        if (UDW_GameInstance* DWGI = Cast<UDW_GameInstance>(GI))
        {
            DWGI->SaveGameData();
        }
    }
}
