#include "EnemyAIController.h"

const FName AEnemyAIController::DefaultPosKey(TEXT("DefaultPos")); // FString으로 Blackboard와 매칭하니 일치시켜서 작업할 것
const FName AEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyAIController::Player(TEXT("Player"));

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> behaviorTree(TEXT("BehaviorTree'/Game/BluePrint/EnemyBehaviorTree.EnemyBehaviorTree'"));
	if (behaviorTree.Succeeded())
	{
		BTree = behaviorTree.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBlackboardData> blackBoard(TEXT("BlackboardData'/Game/BluePrint/EnemyBlackBoard.EnemyBlackBoard'"));
	if (blackBoard.Succeeded())
	{
		BBoard = blackBoard.Object;
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void AEnemyAIController::RunAI()
{
	if (UseBlackboard(BBoard, Blackboard))
	{
		RunBehaviorTree(BTree);
		test++;
		UE_LOG(LogTemp, Log, TEXT("AI Con : %d"), test);
	}
}

void AEnemyAIController::StopAI()
{
	UBehaviorTreeComponent* BTreeComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BTreeComp == nullptr)
		return;
	
	test--;
	UE_LOG(LogTemp, Log, TEXT("AI Con : %d"), test);
	BTreeComp->StopTree(EBTStopMode::Safe);
}
