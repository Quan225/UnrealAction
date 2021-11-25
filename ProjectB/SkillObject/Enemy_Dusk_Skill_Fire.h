#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "Enemy_Dusk_Skill_Fire.generated.h"

UCLASS()
class PROJECTB_API UEnemy_Dusk_Skill_Fire : public USkillObject
{
	GENERATED_BODY()


public:
	UEnemy_Dusk_Skill_Fire();

public:
	virtual void ExecuteSkill() override;
	virtual void ExecuteSkill(class AGameCharacter* Caster) override;

	virtual void CustomSpawnEvent(class AGameCharacter* Caster) override;

	UPROPERTY()
	class AProjectile* SpawnProjectile;
};
