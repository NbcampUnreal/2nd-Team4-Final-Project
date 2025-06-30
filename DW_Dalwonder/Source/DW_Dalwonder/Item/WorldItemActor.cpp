#include "WorldItemActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ItemTranslator.h"
#include "Interactionprompt.h"
#include "Character/DW_CharacterBase.h"
#include "Monster/DW_MonsterBase.h"

AWorldItemActor::AWorldItemActor()
{
    PrimaryActorTick.bCanEverTick = true;

    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    SetRootComponent(RootSceneComponent);

	ItemDataTable = CreateDefaultSubobject<UDataTable>(TEXT("ItemDataTable"));

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(RootSceneComponent);
    MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetupAttachment(RootSceneComponent);
    DetectionSphere->SetSphereRadius(150.f);
    DetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &AWorldItemActor::OnPlayerEnterRadius);
    DetectionSphere->OnComponentEndOverlap.AddDynamic(this, &AWorldItemActor::OnPlayerExitRadius);

    InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
    InteractionWidget->SetupAttachment(RootSceneComponent);
    InteractionWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
    InteractionWidget->SetWidgetSpace(EWidgetSpace::World);
    InteractionWidget->SetDrawSize(FVector2D(200.f, 50.f));
    InteractionWidget->SetVisibility(false);

    InteractionWidget->SetWidgetClass(InteractionWidgetClass);

	ItemBase = CreateDefaultSubobject<UItemBase>(TEXT("ItemBase"));
}

void AWorldItemActor::BeginPlay()
{
    Super::BeginPlay();

    if (!(ItemBase->ItemCode).IsEmpty())
    {
        bool bSuccess = false;

        UItemTranslator::ParseItemCode(
            ItemBase->ItemCode,
            ItemBase->ItemGrade,
            ItemBase->EnchantLevel,
            ItemBase->ItemBaseData.ItemID,
            bSuccess);

        ItemBase->LoadItemFromCode(ItemBase->ItemCode);

#if WITH_EDITOR
        if (!bSuccess)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("월드아이템 비상!!!!")));
        }
#endif
    }

    if (ItemDataTable)
    {
        static const FString ContextString(TEXT("Item Lookup"));
        const TMap<FName, uint8*>& RowMap = ItemDataTable->GetRowMap();

        for (const TPair<FName, uint8*>& RowPair : RowMap)
        {
            const FItemData* RowData = reinterpret_cast<FItemData*>(RowPair.Value);
            if (RowData && RowData->ItemID == ItemBase->ItemBaseData.ItemID)
            {
                ItemBase->ItemBaseData = *RowData;

                if (RowData->ItemMesh.Get())
                {
                    MeshComponent->SetStaticMesh(RowData->ItemMesh.Get());
                }
                else
                {
                    RowData->ItemMesh.LoadSynchronous();
                    if (RowData->ItemMesh.Get())
                    {
                        MeshComponent->SetStaticMesh(RowData->ItemMesh.Get());
                    }
                }

                break;
            }
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
	UE_LOG(LogTemp, Error, TEXT("AWorldItemActor::Interact CALLED!"));
	
    if (!bCanInteract || !PlayerCharacter) return;
	if (OwnerMonster)
	{
		OwnerMonster->DestroySpawnedVFX();
	}
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("아이템과 상호작용!"));
#endif
    Destroy();
}

void AWorldItemActor::SetItemCode(FString NewItemCode)
{
    if (ItemBase)
    {
        ItemBase->ItemCode = NewItemCode;

        bool bSuccess = false;
        UItemTranslator::ParseItemCode(
            ItemBase->ItemCode,
            ItemBase->ItemGrade,
            ItemBase->EnchantLevel,
            ItemBase->ItemBaseData.ItemID,
            bSuccess);

        ItemBase->LoadItemFromCode(ItemBase->ItemCode);

        if (ItemDataTable)
        {
            static const FString ContextString(TEXT("Item Lookup"));
            const TMap<FName, uint8*>& RowMap = ItemDataTable->GetRowMap();

            for (const TPair<FName, uint8*>& RowPair : RowMap)
            {
                const FItemData* RowData = reinterpret_cast<FItemData*>(RowPair.Value);
                if (RowData && RowData->ItemID == ItemBase->ItemBaseData.ItemID)
                {
                    ItemBase->ItemBaseData = *RowData;

                    if (RowData->ItemMesh.Get())
                    {
                        MeshComponent->SetStaticMesh(RowData->ItemMesh.Get());
                    }
                    else
                    {
                        RowData->ItemMesh.LoadSynchronous();
                        if (RowData->ItemMesh.Get())
                        {
                            MeshComponent->SetStaticMesh(RowData->ItemMesh.Get());
                        }
                    }

                    break;
                }
            }
        }

        if (UInteractionprompt* PromptWidget = Cast<UInteractionprompt>(InteractionWidget->GetUserWidgetObject()))
        {
            PromptWidget->OwningWorldItemActor = this;
            PromptWidget->InitializeData();
        }
    }
}

void AWorldItemActor::SetOwnerMonster(ADW_MonsterBase* InOwnerMonster)
{
	OwnerMonster = InOwnerMonster;
}

