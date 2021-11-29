#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "EnemyAIController.h"
#include "PlayerCharacter.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <DrawDebugHelpers.h>
#include "BTService_Detect.generated.h"

UCLASS()
class PROJECTB_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()

public:
    UBTService_Detect();

    UPROPERTY(Category = Service, EditAnywhere, meta = (ClampMin = "0.0"))
    float Sight;
    
    UPROPERTY(Category = Service, EditAnywhere, meta = (ClampMin = "0.0"))
    float IncreaseRateSight;

protected:
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
