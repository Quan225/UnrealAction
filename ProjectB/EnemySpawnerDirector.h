//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "WaveDataTable.h"
//#include "EnemySpawnerDirector.generated.h"
//
//DECLARE_MULTICAST_DELEGATE_OneParam(FSpawnerDelegate, int32);
//
//USTRUCT(BlueprintType)
//struct FWaveEnemyInfo
//{
//	GENERATED_BODY()
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		FString EnemyName;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		int32 SpawnerIndex;
//};
//USTRUCT(BlueprintType)
//struct FWaveInfo
//{
//	GENERATED_BODY()
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		EWaveType WaveType;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		TArray<FWaveEnemyInfo> EnemyInfo;
//};
//
//UCLASS()
//class PROJECTB_API AEnemySpawnerDirector : public AActor
//{
//	GENERATED_BODY()
//	
//public:	
//	AEnemySpawnerDirector();
//
//protected:
//	virtual void BeginPlay() override;
//
//protected:
//	void LoadWaveInfo();
//
//	TMap<int32, FWaveInfo> WaveInfo;
//
//public:
//	void RegistSpawner(class AEnemySpawner* NewSpawner);
//	void DeleteSpawner(class AEnemySpawner* DelSpawner);
//
//	FSpawnerDelegate SpawnDelegate;
//
//protected:
//	TArray<class AEnemySpawner*> Spawners;
//
//protected:
//	int32 AliveEnemyCount;
//	int32 CurSpawnStep;
//
//	UFUNCTION()
//	void SpawnStream();
//	UFUNCTION()
//	void AddAliveEnemy(int32 AddCount) { 
//		AliveEnemyCount += AddCount; 
//		SpawnStream();
//	}
//};
