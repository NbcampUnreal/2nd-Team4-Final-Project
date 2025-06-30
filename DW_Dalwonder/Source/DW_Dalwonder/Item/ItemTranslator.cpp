#include "Item/ItemTranslator.h"

FString UItemTranslator::MakeItemCode(EItemGrade ItemGrade, int32 EnchantLevel, const FString& ItemRowID)
{
    // 유효성 검사
    int32 GradeVal = static_cast<int32>(ItemGrade);
    if (GradeVal < 0 || GradeVal > 9)
    {
#if WITH_EDITOR
        //UE_LOG(LogTemp, Warning, TEXT("MakeItemCode: Invalid ItemGrade (%d), clamping."), GradeVal);
#endif
        GradeVal = FMath::Clamp(GradeVal, 0, 9);
    }

    if (EnchantLevel < 0) EnchantLevel = 0;
    if (EnchantLevel > 99)
    {
#if WITH_EDITOR
        //UE_LOG(LogTemp, Warning, TEXT("MakeItemCode: EnchantLevel (%d) capped at 99."), EnchantLevel);
#endif
        EnchantLevel = 99;
    }

    if (ItemRowID.IsEmpty())
    {
#if WITH_EDITOR
        //UE_LOG(LogTemp, Warning, TEXT("MakeItemCode: ItemRowID is empty."));
#endif
    }

    // 조합된 아이템 코드 문자열 반환
    return FString::Printf(TEXT("%d%02d%s"), GradeVal, EnchantLevel, *ItemRowID);
}

void UItemTranslator::ParseItemCode(
    const FString& ItemCode,
    EItemGrade& OutItemGrade,
    int32& OutEnchantLevel,
    FString& OutItemRowID,
    bool& OutIsSuccess
)
{
    // 기본적으로 실패 상태로 초기화
    OutIsSuccess = false;
    OutItemGrade = EItemGrade::Normal;
    OutEnchantLevel = 0;
    OutItemRowID = TEXT("");

    // 아이템 코드가 규칙에 필요한 최소 길이를 충족하는지 확인
    // 규칙상 최소 2글자(등급, ID 1글자)는 되어야 함
    if (ItemCode.IsEmpty() || ItemCode.Len() < 2) 
    {
        return; // 코드가 너무 짧으면 실패
    }

    // --- 규칙 적용: 2번째 자리수 = 등급 ---
    FString GradeString = ItemCode.Mid(1, 1); // 2번째 글자 (인덱스 1)를 1개 가져옴
    
    // 등급 부분이 숫자인지 확인
    if (!GradeString.IsNumeric())
    {
        return; // 숫자가 아니면 실패
    }
    
    int32 GradeValue = FCString::Atoi(*GradeString); // 문자열을 숫자로 변환

    // 숫자를 EItemGrade enum 타입으로 변환하고 유효성 검사
    const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EItemGrade"));
    if (EnumPtr && EnumPtr->IsValidEnumValue(GradeValue))
    {
        OutItemGrade = static_cast<EItemGrade>(GradeValue);
    }
    else
    {
        return; // 유효하지 않은 등급 값이면 실패
    }


    // --- 다른 정보 파싱 (강화레벨, RowID) ---
    // 이 부분은 현재 규칙에 명확하지 않으므로, ItemCode 전체를 RowID로 간주합니다.
    // 만약 다른 규칙(예: 3, 4번째 자리=강화)이 있다면 이 부분을 수정해야 합니다.
    OutItemRowID = ItemCode;
    OutEnchantLevel = 0; // 강화레벨 규칙이 없으므로 0으로 설정


    // 모든 파싱이 성공적으로 끝났음을 표시
    OutIsSuccess = true;
}