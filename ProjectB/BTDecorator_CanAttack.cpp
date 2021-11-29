#include "BTDecorator_CanAttack.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return false;
	
	AEnemy* enemyCharacter = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (enemyCharacter == nullptr)
		return false;

	if (!enemyCharacter->CanAttack())
		return false;

	APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AEnemyAIController::Player));
	if (nullptr == Target) 
		return false;

	bool bResult = (Target->GetDistanceTo(ControllingPawn) <= AttackRange);
	return bResult;
}

UBTDecorator_CanMove::UBTDecorator_CanMove()
{
	NodeName = TEXT("CanMove");
}

bool UBTDecorator_CanMove::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UE_LOG(LogTemp, Log, TEXT("dddd"));

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return false;

	AEnemy* enemyCharacter = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (enemyCharacter == nullptr)
		return false;

	if (enemyCharacter->GetState() != ECharacterState::Idle)
		return false;

	return true;
}
