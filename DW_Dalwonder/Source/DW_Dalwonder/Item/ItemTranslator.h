#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EItemGrade.h"
#include "ItemTranslator.generated.h"

/**
 * 아이템 코드를 생성 및 해석하는 유틸리티 클래스
 */
UCLASS()
class DW_DALWONDER_API UItemTranslator : public UObject
{
    GENERATED_BODY()

public:
    // 아이템 코드 생성 (FString 기반)
    UFUNCTION(BlueprintCallable, Category = "ItemCode")
    static FString MakeItemCode(EItemGrade ItemGrade, int32 EnchantLevel, const FString& ItemRowID);

    // 아이템 코드 해석 (FString 기반)
    UFUNCTION(BlueprintCallable, Category = "ItemCode")
    static void ParseItemCode(
        const FString& ItemCode,
        EItemGrade& OutItemGrade,
        int32& OutEnchantLevel,
        FString& OutItemRowID,
        bool& OutIsSuccess
    );
};
