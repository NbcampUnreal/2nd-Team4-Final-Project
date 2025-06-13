#include "WorldItemActor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ItemTranslator.h"
#include "Interactionprompt.h"
#include "Character/DW_CharacterBase.h"

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

		ItemBase->LoadItemFromCode(ItemBase->ItemCode);

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

			// 이미 로드되어 있다면 바로 사용
			if (FoundData->ItemMesh.Get())
			{
				if (MeshComponent)
				{
					MeshComponent->SetStaticMesh(FoundData->ItemMesh.Get());
				}
			}
			else // 로드되어 있지 않다면 비동기 로드
			{
				FoundData->ItemMesh.LoadSynchronous(); // BeginPlay에서는 동기 로드가 편리할 수 있습니다.
				// 게임 도중 스폰되는 아이템이라면 비동기 로드를 고려하세요.
				if (MeshComponent && FoundData->ItemMesh.Get())
				{
					MeshComponent->SetStaticMesh(FoundData->ItemMesh.Get());
				}
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
    if (!bCanInteract || !PlayerCharacter) return;
#if WITH_EDITOR
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("아이템과 상호작용!"));
#endif
    Destroy();
}

void AWorldItemActor::SetItemCode(int NewItemCode)
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

        ItemBase->LoadItemFromCode(ItemBase->ItemCode); // 이 함수가 ItemBaseData를 채운다고 가정

#if WITH_EDITOR
        if (!bSuccess) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("월드아이템 ItemCode 파싱 실패!!!! ItemCode: %d"), NewItemCode));
#endif

        // ItemCode가 설정된 후 BeginPlay에서 DataTable을 통해 나머지 데이터를 로드합니다.
        // 하지만 SetItemCode가 호출되는 시점에 이미 DataTable이 로드되어 있다면 여기서 바로 적용할 수도 있습니다.
        if (ItemDataTable)
        {
            FName FItemRowName = FName(*FString::FromInt(ItemBase->ItemBaseData.ItemID));
            const FItemData* FoundData = ItemDataTable->FindRow<FItemData>(FItemRowName, TEXT("Item Lookup"));
            if (FoundData)
            {
                ItemBase->ItemBaseData = *FoundData;
                // 메시 설정 등 아이템 데이터에 따른 추가 설정
                if (FoundData->ItemMesh.Get())
                {
                    if (MeshComponent)
                    {
                        MeshComponent->SetStaticMesh(FoundData->ItemMesh.Get());
                    }
                }
                else // 로드되어 있지 않다면 비동기 로드
                {
                    FoundData->ItemMesh.LoadSynchronous(); // BeginPlay에서는 동기 로드가 편리할 수 있습니다.
                    // 게임 도중 스폰되는 아이템이라면 비동기 로드를 고려하세요.
                    if (MeshComponent && FoundData->ItemMesh.Get())
                    {
                        MeshComponent->SetStaticMesh(FoundData->ItemMesh.Get());
                    }
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

