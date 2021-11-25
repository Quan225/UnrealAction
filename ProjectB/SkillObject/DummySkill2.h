#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "DummySkill2.generated.h"

UCLASS()
class PROJECTB_API UDummySkill2 : public USkillObject
{
	GENERATED_BODY()
public:
	UDummySkill2();

public:
	virtual void ExecuteSkill() override;
	virtual void ExecuteSkill(class AGameCharacter* Caster) override;
};
