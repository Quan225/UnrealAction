#include "UW_Hud.h"

void UUW_Hud::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DisplayCharacter == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Log Message"));
		return;
	}

	HealthBar->SetPercent(DisplayCharacter->GetCurHp() / DisplayCharacter->GetMaxHp());
	StaminaBar->SetPercent(DisplayCharacter->GetCurStamina() / DisplayCharacter->GetMaxStamina());
}