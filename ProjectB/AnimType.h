#pragma once

// NotUse

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

UENUM(BlueprintType)
enum class EAnimType: uint8
{
    Movement UMETA(DisplayName = "Movement"),
    Attack   UMETA(DisplayName = "Attack"),
    Damage   UMETA(DisplayName = "Damage"),
    Emote    UMETA(DisplayName = "Emote")
};