#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UW_Hud.h"
#include "PlayerCharacter.h"
#include "PlayerCharacterController.generated.h"


UCLASS()
class PROJECTB_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()
public:
	APlayerCharacterController();
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<UUW_Hud> HudClass;
	UPROPERTY()
	UUW_Hud* HudObject;

};
