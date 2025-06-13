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

    /** ���� ����� ���̳��� ��Ƽ���� */
    UPROPERTY()
    UMaterialInstanceDynamic* MediaMID;

    /** �� �� �� ������ �������Ʈ���� ������ �� �ֵ��� �صθ� ���մϴ� */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video")
    UMaterialInterface* MediaMaterial = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Video")
    UMediaTexture* MediaTexture = nullptr;

    /** �����̳ʿ��� ���ε��� �̹��� ���� */
    UPROPERTY(meta = (BindWidget))
    UImage* VideoImage = nullptr;
};