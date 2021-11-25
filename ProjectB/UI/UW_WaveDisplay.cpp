#include "UW_WaveDisplay.h"
#include <Components/TextBlock.h>

void UUW_WaveDisplay::NativeConstruct()
{
	Super::NativeConstruct();
}


void UUW_WaveDisplay::PlayWaveStartAnim(int32 WaveStep)
{
	FString DisplayStr = FString::Printf(TEXT("Wave : %d"), WaveStep);
	WaveText->SetText(FText::FromString(DisplayStr));
	PlayAnimation(WaveStart);
}
