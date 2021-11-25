#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerCharacterController.h"
#include "PlayerCharacter.h"
#include "WaveDataTable.h"
#include "ProjectBGameModeBase.generated.h"

UCLASS()
class PROJECTB_API AProjectBGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectBGameModeBase();
	
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void StartPlay() override;

public:
	void SpawnLevelItem(FVector SpawnPos);
	bool PickUpItem(class AItemActor* PickUpItemActor);
	TArray<class AItemActor*> LevelItems;

protected:
	TArray<FWaveDataTable> WaveInfos;
	void LoadWaveInfo();

	int32 CurWaveStep;

public:

	void StartWave();
	void KillEnemy(int32 SubNum);

	TArray<class AEnemySpawner*> EnemySpawners;

protected:
	int32 AliveEnemyNum;

};
