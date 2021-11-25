#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "Player_Skill_Dodge.generated.h"

UCLASS()
class PROJECTB_API UPlayer_Skill_Dodge : public USkillObject
{
	GENERATED_BODY()
public:
	UPlayer_Skill_Dodge();

public:
	virtual void ExecuteSkill(class AGameCharacter* Caster) override;

};
