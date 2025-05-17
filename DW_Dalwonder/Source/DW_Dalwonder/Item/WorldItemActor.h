// WorldItemActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemData.h" // FItemData 정의된 헤더
#include "WorldItemActor.generated.h"

UCLASS()
class DW_DALWONDER_API AWorldItemActor : public AActor
{
    GENERATED_BODY()

public:
    AWorldItemActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 아이템 데이터 이름 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemRowName;

    // 아이템 데이터테이블 
    UPROPERTY(EditDefaultsOnly, Category = "Item")
    UDataTable* ItemDataTable;

    // 아이템 실제 데이터 
    UPROPERTY(EditAnywhere)
    FItemData ItemData;

    // 감지 범위 
    UPROPERTY(VisibleAnywhere)
    class USphereComponent* DetectionSphere;

    // 상호작용 문구 위젯 
    UPROPERTY(VisibleAnywhere)
    class UWidgetComponent* InteractionWidget;

    // 아이템 시각적 표시 
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* MeshComponent;

    // 플레이어 감지 
    UFUNCTION()
    void OnPlayerEnterRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnPlayerExitRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // 플레이어가 상호작용 가능한 상태인지
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    bool bCanInteract = false;

    // 실제 상호작용 처리
    void Interact(class ADW_CharacterBase* PlayerCharacter);

	FItemData GetItemData() const
	{
		return ItemData;
	}
};
