#include "PlayerCharacterController.h"
APlayerCharacterController::APlayerCharacterController()
{
	static ConstructorHelpers::FClassFinder<UUW_Hud> WB_Hud(TEXT("WidgetBlueprint'/Game/UI/WB_Hud.WB_Hud_C'"));
	if (WB_Hud.Succeeded())
	{
		HudClass = WB_Hud.Class;
	}


}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		HudObject = CreateWidget<UUW_Hud>(this, HudClass);
		if (HudObject != nullptr)
		{
			HudObject->AddToViewport();
			HudObject->SetDisplayCharacter(Cast<APlayerCharacter>(GetPawn()));
		}
	}

	//SetShowMouseCursor(true);
	//SetInputMode(FInputModeUIOnly());
}
