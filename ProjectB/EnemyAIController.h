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

public:
	AEnemyAIController();

	void InitAI(UBehaviorTree* InitBTree, UBlackboardData* InitBBorad);

	virtual void OnPossess(APawn* InPawn) override;

	void RunAI();
	void StopAI();

	static const FName DefaultPosKey;
	static const FName PatrolPosKey;
	static const FName Player;
};
