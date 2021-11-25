#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "Player_Skill_Strike.generated.h"

UCLASS()
class PROJECTB_API UPlayer_Skill_Strike : public USkillObject
{
	GENERATED_BODY()

public:
	UPlayer_Skill_Strike();

public:
	virtual void ExecuteSkill() override;
	virtual void ExecuteSkill(class AGameCharacter* Caster) override;

	virtual void CustomHitEvent(class AGameCharacter* Caster) override;
	
};
