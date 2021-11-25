#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "EnemyAIController.h"
#include "PlayerCharacter.h"
#include "Enemy.h"
#include <BehaviorTree/BlackboardComponent.h>
#include "BTDecorator_CanAttack.generated.h"

UCLASS()
class PROJECTB_API UBTDecorator_CanAttack : public UBTDecorator
{
	GENERATED_BODY()
public:
    UBTDecorator_CanAttack();

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
