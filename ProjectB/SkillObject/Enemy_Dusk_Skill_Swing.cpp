#include "Enemy_Dusk_Skill_Swing.h"
#include <ProjectB/Enemy.h>
#include <DrawDebugHelpers.h>

UEnemy_Dusk_Skill_Swing::UEnemy_Dusk_Skill_Swing()
{

}

void UEnemy_Dusk_Skill_Swing::ExecuteSkill()
{
}

void UEnemy_Dusk_Skill_Swing::ExecuteSkill(AGameCharacter* Caster)
{
	AEnemy* CastPlayer = Cast<AEnemy>(Caster);
	if (CastPlayer == nullptr)
		return;

	UAnimMontage* mtg = CastPlayer->DataComponent->FindSkillMontage(SkillName);
	if (mtg == nullptr)
		return;

	CastPlayer->AnimInst->PlaySkillMontage(mtg);
	CastPlayer->SetState(ECharacterState::Attack);

}