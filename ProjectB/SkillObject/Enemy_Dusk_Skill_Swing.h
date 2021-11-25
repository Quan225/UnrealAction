#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "Enemy_Dusk_Skill_Swing.generated.h"

UCLASS()
class PROJECTB_API UEnemy_Dusk_Skill_Swing : public USkillObject
{
	GENERATED_BODY()
public:
	UEnemy_Dusk_Skill_Swing();

public:
	virtual void ExecuteSkill() override;
	virtual void ExecuteSkill(class AGameCharacter* Caster) override;
};
