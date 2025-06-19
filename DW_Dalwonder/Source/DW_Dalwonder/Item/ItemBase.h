#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemData.h"
#include "EItemType.h" 
#include "EItemGrade.h" 
#include "ItemBase.generated.h"


UCLASS(EditInlineNew, BlueprintType, BlueprintType)
class DW_DALWONDER_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
    UItemBase();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ItemCode = 0;

    // 아이템 코드 파싱 후 저장될 실제 아이템 정보
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
    FItemData ItemBaseData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemGrade ItemGrade = EItemGrade::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 EnchantLevel = 0; 

    // ItemCode를 기반으로 아이템 정보를 로드하는 함수
    UFUNCTION(BlueprintCallable, Category = "Item")
    virtual void LoadItemFromCode(int32 InItemCode);


    // --- 아이템 동작 함수들 ---
    // 아이템을 사용할 때 호출되는 가상 함수
    UFUNCTION(BlueprintCallable, Category = "Item Action")
    virtual bool UseItem(AActor* Instigator);

    // 아이템을 장착할 때 호출되는 가상 함수
    UFUNCTION(BlueprintCallable, Category = "Item Action")
    virtual bool EquipItem(AActor* Instigator);

    // 아이템을 해제할 때 호출되는 가상 함수
    UFUNCTION(BlueprintCallable, Category = "Item Action")
    virtual bool UnequipItem(AActor* Instigator);

    // 아이템을 버릴 때 호출되는 가상 함수
    UFUNCTION(BlueprintCallable, Category = "Item Action")
    virtual bool DropItem(AActor* Instigator);

    // 아이템이 스택 가능한지 확인하는 함수
    UFUNCTION(BlueprintPure, Category = "Item")
    bool IsStackable() const { return ItemBaseData.MaxStackSize > 1; }

    // 아이템 타입 확인을 위한 헬퍼 함수
    UFUNCTION(BlueprintPure, Category = "Item")
    EItemType GetItemType() const { return ItemBaseData.ItemType; }
   
};