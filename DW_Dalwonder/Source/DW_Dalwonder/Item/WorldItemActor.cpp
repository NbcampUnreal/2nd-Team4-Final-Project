#include "WorldItemActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/DW_CharacterBase.h"

AWorldItemActor::AWorldItemActor()
    : Super()
{
    PrimaryActorTick.bCanEverTick = true;

	ItemDataTable = CreateDefaultSubobject<UDataTable>(TEXT("ItemDataTable"));

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = MeshComponent;

    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetupAttachment(RootComponent);
    DetectionSphere->SetSphereRadius(150.f);
    DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWorldItemActor::OnPlayerEnterRadius);
    DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AWorldItemActor::OnPlayerExitRadius);

    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(RootComponent);
    InteractionWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
    InteractionWidget->SetWidgetSpace(EWidgetSpace::World);
    InteractionWidget->SetDrawSize(FVector2D(200.f, 50.f));
    InteractionWidget->SetVisibility(false);

}

void AWorldItemActor::BeginPlay()
{
    Super::BeginPlay();

    if (ItemDataTable)
    {
        const FItemData* FoundData = ItemDataTable->FindRow<FItemData>(ItemRowName, TEXT("Item Lookup"));
        if (FoundData)
        {
            ItemData = *FoundData;
        }
    }
}

void AWorldItemActor::OnPlayerEnterRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(OtherActor))
    {
        bCanInteract = true;
        InteractionWidget->SetVisibility(true);
        Player->AddNearbyItem(this);
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("아이템 : %s 등록"), *ItemData.ItemName.ToString()));

    }
}

void AWorldItemActor::OnPlayerExitRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (ADW_CharacterBase* Player = Cast<ADW_CharacterBase>(OtherActor))
    {
        bCanInteract = false;
        InteractionWidget->SetVisibility(false);
        Player->RemoveNearbyItem(this);
       // GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("아이템 : %s 퇴출"), *ItemData.ItemName.ToString()));
    }
}

void AWorldItemActor::Interact(ADW_CharacterBase* PlayerCharacter)
{
    if (!bCanInteract || !PlayerCharacter) return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("아이템과 상호작용!"));
    Destroy();
}
