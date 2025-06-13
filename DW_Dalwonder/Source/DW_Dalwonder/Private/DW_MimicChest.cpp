#include "DW_MimicChest.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "item/WorldItemActor.h"

ADW_MimicChest::ADW_MimicChest()
{
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;

    CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);

    // LineTrace 및 Overlap 감지용 채널 설정
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // 라인트레이스용
    CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);     // 오버랩 감지용
}



void ADW_MimicChest::BeginPlay()
{
    Super::BeginPlay();
}

void ADW_MimicChest::Interact_Implementation(AActor* Interactor)
{
    Super::Interact_Implementation(Interactor);

    if (bHasOpened)
    {
        return;
    }


    bHasOpened = true;

    if (bIsMonster)
    {
        UE_LOG(LogTemp, Error, TEXT("미믹!"));
        SpawnMonster();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("일반 상자네!"));
        PlayOpenAnimation();
        DropItem();
    }

}

void ADW_MimicChest::SpawnMonster()
{
    if (!MonsterClass)
    {
        return;
    }

    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation();

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    UE_LOG(LogTemp, Error, TEXT("상자 삭제하고 몹 스폰!"));

    Destroy();

    AActor* Spawned = GetWorld()->SpawnActor<AActor>(MonsterClass, SpawnLocation, SpawnRotation, Params);

    if (Spawned)
    {
        UE_LOG(LogTemp, Error, TEXT("소환 성공!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("소환 실패!"));
    }
}

void ADW_MimicChest::PlayOpenAnimation()
{
    if (SkeletalMesh && OpenChestMontage)
    {
        UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(OpenChestMontage);
        }
    }
}

void ADW_MimicChest::DropItem()
{
    FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 50.f);
    FActorSpawnParameters Params;

    AWorldItemActor* DroppedItem = GetWorld()->SpawnActor<AWorldItemActor>(AWorldItemActor::StaticClass(), SpawnLocation, FRotator::ZeroRotator, Params);
    if (DroppedItem)
    {
        DroppedItem->SetItemCode(ItemCode);
        UE_LOG(LogTemp, Error, TEXT("탬 드롭!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("탬 드롭 안댐~!"));
    }
}
