#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "DummySkill.generated.h"

UCLASS()
class PROJECTB_API UDummySkill : public USkillObject
{
	GENERATED_BODY()
public:
	UDummySkill();

public:
	virtual void ExecuteSkill() override;
	virtual void ExecuteSkill(class AGameCharacter* Caster) override;
};
