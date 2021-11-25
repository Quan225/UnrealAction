#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/ProgressBar.h>
#include "PlayerCharacter.h"
#include "UW_Hud.generated.h"

UCLASS()
class PROJECTB_API UUW_Hud : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	APlayerCharacter* DisplayCharacter;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* SteminaBar;
public:
	void SetDisplayCharacter(APlayerCharacter* NewDisplayCharacter) { DisplayCharacter = NewDisplayCharacter; }
};
