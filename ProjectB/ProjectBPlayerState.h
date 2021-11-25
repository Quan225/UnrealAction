#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ProjectBPlayerState.generated.h"

UCLASS()
class PROJECTB_API AProjectBPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AProjectBPlayerState();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Transient, Replicated)
	int32 KillCount;
};
