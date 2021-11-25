#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "DummySkill3.generated.h"

UCLASS()
class PROJECTB_API UDummySkill3 : public USkillObject
{
	GENERATED_BODY()
public:
	UDummySkill3();

public:
	virtual void ExecuteSkill() override;
	virtual void ExecuteSkill(class AGameCharacter* Caster) override;
};
