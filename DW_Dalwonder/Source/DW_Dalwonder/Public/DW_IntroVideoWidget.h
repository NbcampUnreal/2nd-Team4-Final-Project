#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DW_IntroVideoWidget.generated.h"

class UImage;
class UMaterialInterface;
class UMediaTexture;

UCLASS()
class DW_DALWONDER_API UDW_IntroVideoWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void SetMediaMaterial(UMaterialInterface* InMaterial);

    UFUNCTION(BlueprintCallable)
    void SetMediaTexture(UMediaTexture* InMediaTexture);

protected:
    virtual void NativeConstruct() override;

    /** 비디오 재생용 다이내믹 머티리얼 */
    UPROPERTY()
    UMaterialInstanceDynamic* MediaMID;

    /** ↓ 이 두 변수는 블루프린트에서 세팅할 수 있도록 해두면 편합니다 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video")
    UMaterialInterface* MediaMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video")
    UMediaTexture* MediaTexture = nullptr;

    /** 디자이너에서 바인딩한 이미지 위젯 */
    UPROPERTY(meta = (BindWidget))
    UImage* VideoImage = nullptr;
};