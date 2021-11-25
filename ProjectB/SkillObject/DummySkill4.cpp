#include "DummySkill4.h"
#include <ProjectB/PlayerCharacter.h>

UDummySkill4::UDummySkill4()
{

}

void UDummySkill4::ExecuteSkill()
{
	UE_LOG(LogTemp, Log, TEXT("Execute dummy Skill"));
}

void UDummySkill4::ExecuteSkill(AGameCharacter* Caster)
{
	UE_LOG(LogTemp, Log, TEXT("Execute dummy Skill2"));
	APlayerCharacter* CastPlayer = Cast<APlayerCharacter>(Caster);
	if (CastPlayer == nullptr)
		return;

	UAnimMontage* mtg = CastPlayer->DataComponent->FindSkillMontage(SkillName);
	if (mtg == nullptr)
		return;

	CastPlayer->AnimInst->PlaySkillMontage(mtg);
	CastPlayer->SetState(ECharacterState::Attack);

}
