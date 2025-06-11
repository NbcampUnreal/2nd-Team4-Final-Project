#include "Item/ItemBase.h"
#include "Item/ItemTranslator.h"

UItemBase::UItemBase()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DT_ItemFinder(TEXT("/Game/Item/Datatables/DT_ItemBaseData.DT_ItemBaseData")); // 에디터 경로 (예시)
    if (DT_ItemFinder.Succeeded())
    {
        ItemDataTable = DT_ItemFinder.Object;
    }
    else
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Warning, TEXT("Failed to find ItemDataTable in UItem constructor. Path might be incorrect or asset not found."));
#endif
	}
}

void UItemBase::LoadItemFromCode(int32 InItemCode)
{
    ItemCode = InItemCode;

    EItemGrade ParsedGrade;
    int32 ParsedEnchantLevel;
    int32 ParsedRowID;
    bool bSuccess;

    // UItemTranslator를 사용하여 ItemCode 파싱
    UItemTranslator::ParseItemCode(ItemCode, ParsedGrade, ParsedEnchantLevel, ParsedRowID, bSuccess);

    if (bSuccess)
    {
        ItemGrade = ParsedGrade;
        EnchantLevel = ParsedEnchantLevel;

        // 데이터 테이블이 유효한지 확인하고 로드
        if (ItemDataTable.IsValid() && ItemDataTable->GetRowStruct() != nullptr)
        {
            // FName으로 변환 (숫자 Row ID를 문자열로)
            FName RowName = FName(*FString::FromInt(ParsedRowID));

            // 데이터 테이블에서 Row 찾기
            const FItemData* FoundData = ItemDataTable->FindRow<FItemData>(RowName, TEXT("Item Loading"));

            if (FoundData)
            {
                ItemBaseData = *FoundData; // 찾은 데이터를 UItem의 ItemBaseData에 복사
                // 필요시 추가 로직: 아이템 타입/등급 Enum 재확인 등
#if WITH_EDITOR
                UE_LOG(LogTemp, Log, TEXT("Item %s (Code: %d) loaded successfully from Data Table. Enchant: %d"), *ItemBaseData.ItemName.ToString(), ItemCode, EnchantLevel);
#endif
			}
            else
            {
#if WITH_EDITOR
                UE_LOG(LogTemp, Warning, TEXT("UItem::LoadItemFromCode: Could not find row '%s' in ItemDataTable for code %d."), *RowName.ToString(), ItemCode);
#endif
				// 기본값 또는 에러 아이템 설정
            }
        }
        else
        {
#if WITH_EDITOR
            UE_LOG(LogTemp, Error, TEXT("UItem::LoadItemFromCode: ItemDataTable is invalid or not set for code %d."), ItemCode);
#endif
		}
    }
    else
    {
#if WITH_EDITOR
        UE_LOG(LogTemp, Error, TEXT("UItem::LoadItemFromCode: Failed to parse ItemCode %d."), InItemCode);
#endif
		// 파싱 실패 시 기본값 또는 에러 처리
    }
}