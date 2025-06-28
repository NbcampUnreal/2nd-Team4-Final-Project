#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EItemGrade.h"
#include "ItemTranslator.generated.h"

/**
 * ������ �ڵ带 ���� �� �ؼ��ϴ� ��ƿ��Ƽ Ŭ����
 */
UCLASS()
class DW_DALWONDER_API UItemTranslator : public UObject
{
    GENERATED_BODY()

public:
    // ������ �ڵ� ���� (FString ���)
    UFUNCTION(BlueprintCallable, Category = "ItemCode")
    static FString MakeItemCode(EItemGrade ItemGrade, int32 EnchantLevel, const FString& ItemRowID);

    // ������ �ڵ� �ؼ� (FString ���)
    UFUNCTION(BlueprintCallable, Category = "ItemCode")
    static void ParseItemCode(
        const FString& ItemCode,
        EItemGrade& OutItemGrade,
        int32& OutEnchantLevel,
        FString& OutItemRowID,
        bool& OutIsSuccess
    );
};
