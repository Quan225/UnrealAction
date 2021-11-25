// Not Use

#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include <Engine/Classes/Animation/AnimSequence.h>
#include <Engine/Classes/Animation/AnimMontage.h>
#include "GameFramework/Actor.h"
#include "AnimType.h"
#include "PlayerAnimData.generated.h"

USTRUCT(BlueprintType)
struct FAnimDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAnimType AnimType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* AnimSequence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString JumpSectionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ComboInputStartTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ComboInputEndTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackStartTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackEndTime;

	// todo : 공격 종류에 따른 공격범위 설정
};

UCLASS()
class PROJECTB_API APlayerAnimData : public AActor
{
	GENERATED_BODY()
	
public:	
	APlayerAnimData();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
