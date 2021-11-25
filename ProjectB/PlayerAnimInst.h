#pragma once

#include "CoreMinimal.h"
#include "CharacterAnimInst.h"
#include "SkillData.h"
#include "PlayerAnimData.h"
#include "PlayerAnimInst.generated.h"

UCLASS()
class PROJECTB_API UPlayerAnimInst : public UCharacterAnimInst
{
    GENERATED_BODY()
public:
    UPlayerAnimInst();

    virtual void NativeBeginPlay() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
};
