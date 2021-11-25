#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "DummySkill4.generated.h"

UCLASS()
class PROJECTB_API UDummySkill4 : public USkillObject
{
	GENERATED_BODY()
public:
	UDummySkill4();

public:
	virtual void ExecuteSkill() override;
	virtual void ExecuteSkill(class AGameCharacter* Caster) override;
	
};
