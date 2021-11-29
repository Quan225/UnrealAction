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
    
    UPROPERTY(Category = Service, EditAnywhere, meta = (ClampMin = "0.0"))
    float AttackRange;

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

UCLASS()
class PROJECTB_API UBTDecorator_CanMove : public UBTDecorator
{
    GENERATED_BODY()
public:
    UBTDecorator_CanMove();

protected:
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
