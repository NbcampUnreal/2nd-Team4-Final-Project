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



    // --- 아이템 정보 획득 헬퍼 함수 ---
    // 아이템이 스택 가능한지 확인하는 함수
    UFUNCTION(BlueprintPure, Category = "Item")
    EItemType GetItemType() const { return ItemBaseData.ItemType; }

    // 아이템의 이름 텍스트를 반환하는 함수
    UFUNCTION(BlueprintPure, Category = "Item Info")
    FText GetItemName() const { return ItemBaseData.ItemName; }

    // 아이템의 설명을 반환하는 함수
    UFUNCTION(BlueprintPure, Category = "Item Info")
    FText GetItemDescription() const { return ItemBaseData.Description; }

    // 아이템의 아이콘 텍스처를 반환하는 함수
    UFUNCTION(BlueprintPure, Category = "Item Info")
    UTexture2D* GetItemIcon() const { return ItemBaseData.Icon.Get(); }

    // 아이템의 무게를 반환하는 함수
    UFUNCTION(BlueprintPure, Category = "Item Info")
    float GetItemWeight() const { return ItemBaseData.Weight; }

    // 아이템의 무게를 반환하는 함수
    UFUNCTION(BlueprintPure, Category = "Item Info")
    EEquipSlotType GetItemSlotType() const { return ItemBaseData.EquipSlot; }

    // 아이템의 최대 스택 사이즈를 반환하는 함수
    UFUNCTION(BlueprintPure, Category = "Item Info")
    int32 GetMaxStackSize() const { return ItemBaseData.MaxStackSize; }
   
};