// WorldItemActor.h

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ItemData.h"
#include "EItemGrade.h"
#include "ItemBase.h"
#include "WorldItemActor.generated.h"

class UWidgetComponent;
class UStaticMeshComponent;

UCLASS()
class DW_DALWONDER_API AWorldItemActor : public AActor
{
    GENERATED_BODY()

public:
    AWorldItemActor();

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced, Category = "Item")
	UItemBase* ItemBase;

    // 아이템 데이터테이블 
    UPROPERTY(EditAnywhere, Category = "Item")
    UDataTable* ItemDataTable;

    // 상호작용 문구 위젯 (이 UWidgetComponent는 이제 UInteractionprompt 타입의 위젯을 생성할 겁니다)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    UWidgetComponent* InteractionWidget;

    // **새로 추가된 부분**: 에디터에서 설정할 Blueprint 위젯 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
    TSubclassOf<UUserWidget> InteractionWidgetClass; // 여기에 UInteractionprompt의 Blueprint 클래스를 지정합니다.
    
    // 아이템 시각적 표시 
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    // 감지 범위 
    UPROPERTY(VisibleAnywhere)
    class USphereComponent* DetectionSphere;

    void SetItemCode(int NewItemCode);

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
};
