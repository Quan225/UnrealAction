#include "EnemySkillComponent.h"

UEnemySkillComponent::UEnemySkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UEnemySkillComponent::BeginPlay()
{
	Super::BeginPlay();
}