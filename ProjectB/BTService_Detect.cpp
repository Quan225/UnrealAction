#include "BTService_Detect.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
		return;

	UWorld* World = ControllingPawn->GetWorld();
	if (World == nullptr)
		return;

	// todo : Radius는 Enemy에서 받아서 각각 다르게...
	FVector center = ControllingPawn->GetActorLocation();
	float detectRadius = 1000.0f;

	TArray<FOverlapResult> overlapResults;
	// todo : Enemy Tag를 사용하여 Ignore하도록 변경할 것
	FCollisionQueryParams collParam(NAME_None, false, ControllingPawn);
	bool isOverlap = World->OverlapMultiByChannel(
		overlapResults,
		center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(detectRadius),
		collParam
	);


	// 셀렉터가 왜 실패로 잡히지..

	if (isOverlap)
	{
		for (const auto& result : overlapResults)
		{
			APlayerCharacter* player = Cast<APlayerCharacter>(result.GetActor());
			if (player != nullptr && player->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::Player, player);
				
				// 디버깅 용.
				if (true)
				{
					DrawDebugSphere(World, center, detectRadius, 16, FColor::Green, false, 0.2f);
					DrawDebugPoint(World, player->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
					DrawDebugLine(World, ControllingPawn->GetActorLocation(), player->GetActorLocation(), FColor::Blue, false, 0.2f);
				}
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::Player, nullptr);
	}

	if (true)
	{
		DrawDebugSphere(World, center, detectRadius, 16, FColor::Red, false, 0.2f);
	}
}
