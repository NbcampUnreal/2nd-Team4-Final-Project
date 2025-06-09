#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h" // 데이터 테이블 로딩을 위해 필요
#include "ItemData.h" // 아이템 데이터 구조체 포함
#include "EItemType.h" 
#include "EItemGrade.h" 
#include "ItemBase.generated.h"


UCLASS(EditInlineNew, BlueprintType)
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
    int32 EnchantLevel; 

    // ItemCode를 기반으로 아이템 정보를 로드하는 함수
    UFUNCTION(BlueprintCallable, Category = "Item")
    virtual void LoadItemFromCode(int32 InItemCode);

protected:
    // 아이템 데이터 테이블 참조 (에디터에서 설정)
    UPROPERTY(EditDefaultsOnly, Category = "Item Data")
    TSoftObjectPtr<UDataTable> ItemDataTable; // 소프트 포인터로 런타임 로딩 최적화
   
};