#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimInst.h"
#include "EnemyAnimInst.generated.h"

UCLASS()
class PROJECTB_API UEnemyAnimInst : public UCharacterAnimInst
{
	GENERATED_BODY()
public:
    UEnemyAnimInst();

    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UAnimSequence*> AnimSeqArr;

};
