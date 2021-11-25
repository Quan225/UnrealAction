//#include "EnemySpawnerDirector.h"
//#include "EnemySpawner.h"
//#include <Kismet/GameplayStatics.h>
//
//AEnemySpawnerDirector::AEnemySpawnerDirector()
//{
//	PrimaryActorTick.bCanEverTick = false;
//	LoadWaveInfo();
//	
//	CurSpawnStep = 0;
//	AliveEnemyCount = 0;
//
//
//	SpawnDelegate.AddUObject(this, &AEnemySpawnerDirector::AddAliveEnemy);
//
//}
//
//void AEnemySpawnerDirector::BeginPlay()
//{
//	Super::BeginPlay();
//
//	TArray<AActor*> AllActors;
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), AllActors);
//
//	for (int i = 0; i < AllActors.Num(); i++)
//	{
//		AEnemySpawner* Spawner = Cast<AEnemySpawner>(AllActors[i]);
//		if (Spawner == nullptr)
//			continue;
//
//		Spawners.Add(Spawner);
//	}
//
//	SpawnStream();
//}
//
//void AEnemySpawnerDirector::LoadWaveInfo()
//{
//	static ConstructorHelpers::FObjectFinder<UDataTable> tb(TEXT("DataTable'/Game/BluePrint/WaveData.WaveData'"));
//	if (!tb.Succeeded())
//		return;
//
//	FString context;
//	TArray<FWaveDataTable*> WaveTable;
//	tb.Object->GetAllRows(context, WaveTable);
//
//	for (int32 i = 0; i < WaveTable.Num(); i++) {
//		if (WaveInfo.Contains(WaveTable[i]->WaveStep))
//		{
//			FWaveEnemyInfo TempEnemyInfo;
//			TempEnemyInfo.EnemyName = WaveTable[i]->EnemyName;
//			TempEnemyInfo.SpawnerIndex = WaveTable[i]->SpawnerIndex;
//
//			WaveInfo[WaveTable[i]->WaveStep].EnemyInfo.Add(TempEnemyInfo);
//		}
//		else
//		{
//			FWaveInfo TempInfo;
//			TempInfo.WaveType = WaveTable[i]->WaveType;
//			
//			FWaveEnemyInfo TempEnemyInfo;
//			TempEnemyInfo.EnemyName = WaveTable[i]->EnemyName;
//			TempEnemyInfo.SpawnerIndex = WaveTable[i]->SpawnerIndex;
//
//			TempInfo.EnemyInfo.Add(TempEnemyInfo);
//
//			WaveInfo.Add(WaveTable[i]->WaveStep, TempInfo);
//		}
//	}
//
//	return;
//}
//
//void AEnemySpawnerDirector::RegistSpawner(AEnemySpawner* NewSpawner)
//{
//	if (NewSpawner == nullptr)
//		return;
//
//	Spawners.Add(NewSpawner);
//}
//
//void AEnemySpawnerDirector::DeleteSpawner(AEnemySpawner* DelSpawner)
//{
//	Spawners.Remove(DelSpawner);
//}
//
//void AEnemySpawnerDirector::SpawnStream()
//{
//	if (GetLocalRole() < ROLE_Authority)
//	{
//		return;
//	}
//
//	int32 NextSpawnStep = CurSpawnStep + 1;
//
//	if (WaveInfo[NextSpawnStep].WaveType == EWaveType::KillAll)
//	{
//		if (AliveEnemyCount > 0)
//			return;
//
//		CurSpawnStep = NextSpawnStep;
//
//		if (CurSpawnStep >= WaveInfo.Num())
//			CurSpawnStep = 0;
//
//		AliveEnemyCount = 0;
//
//		for (const auto& WI : WaveInfo[NextSpawnStep].EnemyInfo)
//		{
//			if (WI.SpawnerIndex < 0 || WI.SpawnerIndex >= Spawners.Num())
//				continue;
//
//			Spawners[WI.SpawnerIndex]->SpawnEnemy();
//		}
//
//	}
//	else
//	{
//
//	}
//}