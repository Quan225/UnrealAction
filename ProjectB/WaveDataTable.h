#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/DataTable.h>
#include "WaveDataTable.generated.h"

//UENUM(BlueprintType)
//enum class EWaveType : uint8
//{
//	// ���̺갡 ����Ǵ� ���
//	Timer = 0, // �ð��� �帧�� ����
//	KillAll = 1 // �������� �� ��� ���͸� ó������ ���
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
	// �� ���̺� ���� ����
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
