#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardData.h>
#include "EnemyAIController.generated.h"

UCLASS()
class PROJECTB_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
private:
	UBehaviorTree* BTree;
	UBlackboardData* BBoard;
	int32 test;

public:
	AEnemyAIController();

	// 컨트롤러가 Pawn/Character를 소유할 때 호출
	virtual void OnPossess(APawn* InPawn) override;

	void RunAI();
	void StopAI();

	static const FName DefaultPosKey;
	static const FName PatrolPosKey;
	static const FName Player;
};
