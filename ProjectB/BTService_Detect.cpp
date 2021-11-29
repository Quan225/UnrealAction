#include "BTService_Detect.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
	Sight = 1000.0f;
	IncreaseRateSight = 100.0f;
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

	Sight = Sight + IncreaseRateSight * DeltaSeconds;
	FVector center = ControllingPawn->GetActorLocation();

	TArray<FOverlapResult> overlapResults;

	FCollisionQueryParams collParam(NAME_None, false, ControllingPawn);
	bool isOverlap = World->OverlapMultiByChannel(
		overlapResults,
		center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel5,
		FCollisionShape::MakeSphere(Sight),
		collParam
	);

	if (isOverlap)
	{
		for (const auto& result : overlapResults)
		{
			APlayerCharacter* player = Cast<APlayerCharacter>(result.GetActor());
			if (player != nullptr && player->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::Player, player);
				
				// µð¹ö±ë ¿ë.
				if (true)
				{
					DrawDebugSphere(World, center, Sight, 16, FColor::Green, false, 0.2f);
					DrawDebugPoint(World, player->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
					DrawDebugLine(World, ControllingPawn->GetActorLocation(), player->GetActorLocation(), FColor::Blue, false, 0.2f);
				}
				return;
			}
		}
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AEnemyAIController::Player, nullptr);
	}

	DrawDebugSphere(World, center, Sight, 16, FColor::Red, false, 0.2f);
}
