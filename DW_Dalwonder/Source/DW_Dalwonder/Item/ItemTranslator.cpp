#include "Item/ItemTranslator.h"

int32 UItemTranslator::MakeItemCode( EItemGrade ItemGrade, int32 EnchantLevel, int32 ItemRowID)
{
    // 유효성 검사

    int32 GradeVal = static_cast<int32>(ItemGrade);
    if (GradeVal < 0 || GradeVal > 9) // ItemGrade를 0~9 (한 자리)로 제한
    {
        UE_LOG(LogTemp, Error, TEXT("MakeItemCode: Invalid ItemGrade (%d) for single digit. Capping."), GradeVal);
        GradeVal = FMath::Clamp(GradeVal, 0, 9); // 강제로 한 자리 숫자로 클램핑
    }

    if (EnchantLevel < 0) EnchantLevel = 0;
    if (EnchantLevel > 99) // 강화 수치를 2자리로 제한 (00~99)
    {
        UE_LOG(LogTemp, Warning, TEXT("MakeItemCode: EnchantLevel (%d) is too high, capping at 99."), EnchantLevel);
        EnchantLevel = 99;
    }
    if (ItemRowID < 0) ItemRowID = 0;


    // FString을 사용하여 각 부분을 조합하고 최종적으로 int32로 변환
    FString ItemCodeStr = FString::Printf(TEXT("%d%02d%d"),
        static_cast<int32>(ItemGrade),      // 아이템 등급 (1자리)
        EnchantLevel,                       // 강화 수치 (항상 2자리로 포맷팅)
        ItemRowID                           // 아이템 Row ID (가변 길이)
    );

    if (ItemCodeStr.Len() > 10) // int32의 최대 길이(약 10자리)를 초과할 경우
    {
        UE_LOG(LogTemp, Error, TEXT("MakeItemCode: Generated ItemCode string \"%s\" is too long for int32. Returning 0."), *ItemCodeStr);
        return 0; // 또는 -1 등 오류 값 반환
    }

    // FString을 int32로 변환
    return FCString::Atoi(*ItemCodeStr);
}

void UItemTranslator::ParseItemCode(int32 ItemCode, EItemGrade& OutItemGrade, int32& OutEnchantLevel, int32& OutItemRowID, bool& OutIsSuccess)
{
    OutIsSuccess = false; // 기본적으로 실패로 설정
    OutItemGrade = EItemGrade::Normal; // 기본값
    OutEnchantLevel = 0;               // 기본값
    OutItemRowID = 0;                   // 기본값

    FString ItemCodeStr = FString::FromInt(ItemCode);

    // 최소 4자리 필요: 1(등급) + 2(강화) + 1(RowID 최소)
    if (ItemCodeStr.Len() < 4)
    {
        UE_LOG(LogTemp, Warning, TEXT("ParseItemCode: ItemCode (%d) is too short to parse (min 5 digits expected)."), ItemCode);
        return;
    }

    
    // 1. 아이템 등급 (첫 번째 자리)
    int32 ParsedItemGradeInt = FCString::Atoi(*ItemCodeStr.Mid(0, 1));
    OutItemGrade = static_cast<EItemGrade>(ParsedItemGradeInt);

    // 유효성 검사: UEnum::IsValidEnumValue 사용
    const UEnum* ItemGradeEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemGrade"), true);
    if (!ItemGradeEnum || !ItemGradeEnum->IsValidEnumValue(ParsedItemGradeInt))
    {
        UE_LOG(LogTemp, Warning, TEXT("ParseItemCode: Invalid ItemGrade (%d) parsed from code %d. Defaulting to EItemGrade::Normal."), ParsedItemGradeInt, ItemCode);
        OutItemGrade = EItemGrade::Normal; // 유효하지 않으면 기본값으로 설정
    }

    // 2. 강화 수치 (두 번째부터 두 자리)
    OutEnchantLevel = FCString::Atoi(*ItemCodeStr.Mid(1, 2));
    // 강화 수치 유효성 검사 (0-99 범위)
    if (OutEnchantLevel < 0 || OutEnchantLevel > 99)
    {
        UE_LOG(LogTemp, Warning, TEXT("ParseItemCode: Invalid EnchantLevel (%d) parsed from code %d. Defaulting to 0."), OutEnchantLevel, ItemCode);
        OutEnchantLevel = 0;
    }

    // 3. 아이템 Row ID (네 번째부터 마지막 자리)
    OutItemRowID = FCString::Atoi(*ItemCodeStr.Mid(3)); 

    OutIsSuccess = true;
}

