#pragma once

#include "CoreMinimal.h"
#include "CharacterSkillComponent.h"
#include "EnemySkillComponent.generated.h"

UCLASS()
class PROJECTB_API UEnemySkillComponent : public UCharacterSkillComponent
{
	GENERATED_BODY()

public:
	UEnemySkillComponent();
protected:
	virtual void BeginPlay() override;
};
