#include "ProjectBPlayerState.h"

AProjectBPlayerState::AProjectBPlayerState()
{
	KillCount = 0;
}

void AProjectBPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectBPlayerState, KillCount);
}
