#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_WaveDisplay.generated.h"

UCLASS()
class PROJECTB_API UUW_WaveDisplay : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;

protected:
	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* WaveStart;

	UPROPERTY(Meta = (BindWidget), Transient)
	class UTextBlock* WaveText;


public:
	void PlayWaveStartAnim(int32 WaveStep);
};
