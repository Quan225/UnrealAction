#include "ProjectBGameModeBase.h"
#include "Inventory/ItemActor.h"
#include <Kismet/GameplayStatics.h>
#include "EnemySpawner.h"
#include "ProjectBGameStateBase.h"
#include "ProjectBPlayerState.h"

AProjectBGameModeBase::AProjectBGameModeBase()
{
	GameStateClass = AProjectBGameStateBase::StaticClass();
	PlayerStateClass = AProjectBPlayerState::StaticClass();
	PlayerControllerClass = APlayerCharacterController::StaticClass();
	DefaultPawnClass = APlayerCharacter::StaticClass();

	LoadWaveInfo();

	AliveEnemyNum = 0;
	CurWaveStep = 0;
}

void AProjectBGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("New Player Login"));
}

void AProjectBGameModeBase::StartPlay()
{
	Super::StartPlay();
	

	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), AllActors);

	for (int i = 0; i < AllActors.Num(); i++)
	{
		AEnemySpawner* Spawner = Cast<AEnemySpawner>(AllActors[i]);
		if (Spawner == nullptr)
			continue;

		EnemySpawners.Add(Spawner);
	}
	StartWave();
}

void AProjectBGameModeBase::SpawnLevelItem(FVector SpawnPos)
{
	AItemActor* NewItem = GetWorld()->SpawnActor<AItemActor>(SpawnPos, FRotator::ZeroRotator);
	NewItem->InitItemData(2);

	LevelItems.Add(NewItem);
}

bool AProjectBGameModeBase::PickUpItem(AItemActor* PickUpItemActor)
{
	int32 FindIdx = LevelItems.Find(PickUpItemActor);
	if (FindIdx != INDEX_NONE)
	{
		LevelItems.RemoveAt(FindIdx);
		PickUpItemActor->Destroy();
		return true;
	}
	return false;
}

void AProjectBGameModeBase::LoadWaveInfo()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> tb(TEXT("DataTable'/Game/BluePrint/WaveData.WaveData'"));
	if (!tb.Succeeded())
		return;

	FString context;
	TArray<FWaveDataTable*> WaveTable;
	tb.Object->GetAllRows(context, WaveTable);

	for (int32 i = 0; i < WaveTable.Num(); i++) {
		FWaveDataTable TempWaveInfo;
		TempWaveInfo.WaveInfo = WaveTable[i]->WaveInfo;
		
		WaveInfos.Add(TempWaveInfo);
	}
}

void AProjectBGameModeBase::StartWave()
{
	CurWaveStep++;

	GetGameState<AProjectBGameStateBase>()->Multi_AddWaveStep(CurWaveStep);

	if (HasAuthority())
	{
		FTimerHandle SpawnTimer;

		GetWorld()->GetTimerManager().SetTimer(SpawnTimer, FTimerDelegate::CreateLambda([&]()
			{
				if (WaveInfos.IsValidIndex(CurWaveStep-1))
				{
					if (!WaveInfos[CurWaveStep - 1].WaveInfo.IsValidIndex(0))
					{
						UE_LOG(LogTemp, Log, TEXT("Wave %d : No enemy information set"), CurWaveStep);
						StartWave();
						return;
					}

					if (!EnemySpawners.IsValidIndex(0))
					{
						UE_LOG(LogTemp, Log, TEXT("No enemy spawner set"));
						return;
					}

					int32 SpawnerNum = 0;
					for (const auto& EnemyInfo : WaveInfos[CurWaveStep - 1].WaveInfo)
					{
						for (int32 i = 0; i < EnemyInfo.Count; i++)
						{
							EnemySpawners[SpawnerNum]->SpawnEnemy(EnemyInfo.EnemyName);
							AliveEnemyNum++;
							SpawnerNum++;

							if (SpawnerNum == EnemySpawners.Num())
								SpawnerNum = 0;
						}
					}
				}
			}), 2.0f, false);
	}
}

void AProjectBGameModeBase::KillEnemy(int32 SubNum)
{
	AliveEnemyNum -= SubNum;

	if (AliveEnemyNum <= 0)
		StartWave();
}
