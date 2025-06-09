#include "WorldItemActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ItemTranslator.h"
#include "Character/DW_CharacterBase.h"

AWorldItemActor::AWorldItemActor()
{
    PrimaryActorTick.bCanEverTick = true;

	ItemDataTable = CreateDefaultSubobject<UDataTable>(TEXT("ItemDataTable"));

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(MeshComponent);
    MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetupAttachment(MeshComponent);
    DetectionSphere->SetSphereRadius(150.f);
    DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWorldItemActor::OnPlayerEnterRadius);
    DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AWorldItemActor::OnPlayerExitRadius);

    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(MeshComponent);
    InteractionWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
    InteractionWidget->SetWidgetSpace(EWidgetSpace::World);
    InteractionWidget->SetDrawSize(FVector2D(200.f, 50.f));
    InteractionWidget->SetVisibility(false);

	ItemBase = CreateDefaultSubobject<UItemBase>(TEXT("ItemBase"));
}

void AWorldItemActor::BeginPlay()
{
    Super::BeginPlay();

	if (ItemBase->ItemCode != 0)
	{
		bool bSuccess = false;
		// 아이템 코드가 설정되어 있다면 해당 아이템 데이터를 로드
        UItemTranslator::ParseItemCode(
            ItemBase->ItemCode,
            ItemBase->ItemGrade,
            ItemBase->EnchantLevel,
            ItemBase->ItemBaseData.ItemID,
            bSuccess);

#if WITH_EDITOR
        if(!bSuccess) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("월드아이템 비상!!!!")));
#endif
	}
	

    if (ItemDataTable)
    {
		FName FItemRowName = FName(*FString::FromInt(ItemBase->ItemBaseData.ItemID));
        const FItemData* FoundData = ItemDataTable->FindRow<FItemData>(FItemRowName, TEXT("Item Lookup"));
        if (FoundData)
        {
            ItemBase->ItemBaseData = *FoundData;
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
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("아이템과 상호작용!"));
#endif
    Destroy();
}
