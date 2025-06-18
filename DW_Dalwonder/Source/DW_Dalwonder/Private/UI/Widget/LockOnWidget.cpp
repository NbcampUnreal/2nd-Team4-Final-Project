#include "UI/Widget/LockOnWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/ScaleBox.h"

void ULockOnWidget::SetMarkerSizeByDistance(float Distance)
{
	const float MinSize = 15.f;
	const float MaxSize = 100.f;
	const float MaxDistance = 3000.f;

	float SizeRatio = 1.f - FMath::Clamp(Distance / MaxDistance, 0.f, 1.f);
	float NewSize = FMath::Lerp(MinSize, MaxSize, SizeRatio);

	if (UScaleBox* ScaleBox = Cast<UScaleBox>(GetWidgetFromName("LockOnScaleBox")))
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ScaleBox->Slot))
		{
			CanvasSlot->SetSize(FVector2D(NewSize, NewSize));
		}
	}
}
