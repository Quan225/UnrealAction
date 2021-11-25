#include "BTTask_EnemyTask.h"

UBTTask_Attack::UBTTask_Attack() { }

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AEnemy* controllingCharacter = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == controllingCharacter)
		return EBTNodeResult::Failed;

	controllingCharacter->Attack();

	return EBTNodeResult::Succeeded;
}



UBTTask_MoveToTarget::UBTTask_MoveToTarget() { }

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AEnemyAIController::DefaultPosKey);
	FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 1000.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AEnemyAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}