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

UBTTask_ClearTarget::UBTTask_ClearTarget() { }

EBTNodeResult::Type UBTTask_ClearTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::Player, nullptr);
	return EBTNodeResult::Succeeded;
}

UBTTask_RotateToTarget::UBTTask_RotateToTarget() { }

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player);
	if (Target == nullptr)
		return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(Target);
	if(TargetActor == nullptr)
		return EBTNodeResult::Failed;

	AActor* ControllingCharacter = Cast<AActor>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingCharacter == nullptr)
		return EBTNodeResult::Failed;

	FVector RotVector = TargetActor->GetActorLocation() - ControllingCharacter->GetActorLocation();

	ControllingCharacter->SetActorRotation(RotVector.Rotation());


	return EBTNodeResult::Succeeded;
}
