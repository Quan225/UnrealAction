#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "WaveDataTable.generated.h"

//UENUM(BlueprintType)
//enum class EWaveType : uint8
//{
//	// 웨이브가 진행되는 방식
//	Timer = 0, // 시간의 흐름에 따라서
//	KillAll = 1 // 스테이지 내 모든 몬스터를 처리했을 경우
//};

USTRUCT(BlueprintType)
struct FWaveEnemyInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EnemyName;
};

USTRUCT(BlueprintType)
struct FWaveDataTable : public FTableRowBase
{
	// 적 웨이브 관련 정보
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FWaveEnemyInfo> WaveInfo;
};

UCLASS()
class PROJECTB_API AWaveData : public AActor
{
	GENERATED_BODY()
	
public:	
	AWaveData();


};
