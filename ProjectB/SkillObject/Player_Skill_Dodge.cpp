#include "Player_Skill_Dodge.h"

UPlayer_Skill_Dodge::UPlayer_Skill_Dodge()
{

}

void UPlayer_Skill_Dodge::ExecuteSkill(AGameCharacter* Caster)
{
	APlayerCharacter* CastPlayer = Cast<APlayerCharacter>(Caster);
	if (CastPlayer == nullptr)
		return;

	UAnimMontage* mtg = CastPlayer->DataComponent->FindSkillMontage(SkillName);
	if (mtg == nullptr)
		return;

	if (CastPlayer->SetState(ECharacterState::Dodge))
	{
		CastPlayer->AnimInst->PlaySkillMontage(mtg);
	}
}
