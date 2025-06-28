#include "Item/ItemTranslator.h"

FString UItemTranslator::MakeItemCode(EItemGrade ItemGrade, int32 EnchantLevel, const FString& ItemRowID)
{
    // 유효성 검사
    int32 GradeVal = static_cast<int32>(ItemGrade);
    if (GradeVal < 0 || GradeVal > 9)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("MakeItemCode: Invalid ItemGrade (%d), clamping."), GradeVal);
#endif
        GradeVal = FMath::Clamp(GradeVal, 0, 9);
    }

    if (EnchantLevel < 0) EnchantLevel = 0;
    if (EnchantLevel > 99)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("MakeItemCode: EnchantLevel (%d) capped at 99."), EnchantLevel);
#endif
        EnchantLevel = 99;
    }

    if (ItemRowID.IsEmpty())
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("MakeItemCode: ItemRowID is empty."));
#endif
    }

    // 조합된 아이템 코드 문자열 반환
    return FString::Printf(TEXT("%d%02d%s"), GradeVal, EnchantLevel, *ItemRowID);
}

void UItemTranslator::ParseItemCode(const FString& ItemCode, EItemGrade& OutItemGrade, int32& OutEnchantLevel, FString& OutItemRowID, bool& OutIsSuccess)
{
    OutIsSuccess = false;
    OutItemGrade = EItemGrade::Normal;
    OutEnchantLevel = 0;
    OutItemRowID = TEXT("");

    if (ItemCode.Len() < 4)
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("ParseItemCode: ItemCode \"%s\" too short."), *ItemCode);
#endif
        return;
    }

    // 등급 (1자리)
    int32 GradeInt = FCString::Atoi(*ItemCode.Mid(0, 1));
    OutItemGrade = static_cast<EItemGrade>(GradeInt);

    const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemGrade"), true);
    if (!EnumPtr || !EnumPtr->IsValidEnumValue(GradeInt))
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("ParseItemCode: Invalid grade parsed: %d"), GradeInt);
#endif
        OutItemGrade = EItemGrade::Normal;
    }

    // 강화 수치 (2자리)
    OutEnchantLevel = FCString::Atoi(*ItemCode.Mid(1, 2));
    if (OutEnchantLevel < 0 || OutEnchantLevel > 99)
    {
        OutEnchantLevel = 0;
    }

    // 나머지 문자열 = ItemRowID
    OutItemRowID = ItemCode.Mid(3);
    OutIsSuccess = true;
}
