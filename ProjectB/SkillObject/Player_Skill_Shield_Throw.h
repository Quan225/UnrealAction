#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "Player_Skill_Shield_Throw.generated.h"

UCLASS()
class PROJECTB_API UPlayer_Skill_Shield_Throw : public USkillObject
{
	GENERATED_BODY()
public:
	UPlayer_Skill_Shield_Throw();

public:
	virtual void ExecuteSkill() override;
	virtual void ExecuteSkill(class AGameCharacter* Caster) override;

	virtual void CustomSpawnEvent(class AGameCharacter* Caster) override;

private:
	UPROPERTY()
	class AEnemy* Target;
	UPROPERTY()
	class AProjectile* SpawnProjectile;
};
