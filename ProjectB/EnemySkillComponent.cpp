#include "EnemySkillComponent.h"

UEnemySkillComponent::UEnemySkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UEnemySkillComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadSkillClass();
}

void UEnemySkillComponent::LoadSkillClass()
{
	Super::LoadSkillClass();

	//AddChainData(EChainType::None, EChainType::Basic_1, EAxisInputType::None, EButtonInputType::None, "Dusk_Gun", "Fire", 1);
	//SkillClass = Instance->GetSkillClass("Player_Wraith");
}