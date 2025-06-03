#include "DW_IntroVideoWidget.h"
#include "Components/Image.h"
#include "MediaTexture.h"
#include "MediaPlayer.h"
#include "Materials/MaterialInstanceDynamic.h"

void UDW_IntroVideoWidget::SetMediaMaterial(UMaterialInterface* InMaterial)
{
    MediaMaterial = InMaterial;
}

void UDW_IntroVideoWidget::SetMediaTexture(UMediaTexture* InMediaTexture)
{
    MediaTexture = InMediaTexture;
}

void UDW_IntroVideoWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (MediaMaterial && MediaTexture && VideoImage)
	{
		UE_LOG(LogTemp, Error, TEXT("O 위젯: MediaMaterial, MediaTexture, VideoImage 모두 유효"));

		MediaMID = UMaterialInstanceDynamic::Create(MediaMaterial, this);
		if (MediaMID)
		{
			MediaMID->SetTextureParameterValue(TEXT("VideoTexture"), Cast<UTexture>(MediaTexture));

			FSlateBrush Brush;
			Brush.SetResourceObject(MediaMID);
			Brush.ImageSize = FVector2D(1920, 1080);
			VideoImage->SetBrush(Brush);

			UE_LOG(LogTemp, Error, TEXT("O 위젯: Dynamic Material 적용 완료"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("X 위젯: Dynamic Material 생성 실패"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("X 위젯: 필수 요소 누락 - MediaMaterial: %s, MediaTexture: %s, VideoImage: %s"),
			*FString(MediaMaterial ? TEXT("O") : TEXT("X")),
			*FString(MediaTexture ? TEXT("O") : TEXT("X")),
			*FString(VideoImage ? TEXT("O") : TEXT("X")));
	}
}
