#include "CharacterAnimInst.h"

UCharacterAnimInst::UCharacterAnimInst()
{
	
}

void UCharacterAnimInst::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCharacterAnimInst::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	MoveSpeed = GetOwningActor()->GetVelocity().Size();
}

void UCharacterAnimInst::StopAllMontage()
{
	StopAllMontages(0.0f);
}

void UCharacterAnimInst::PlaySkillMontage(UAnimMontage* playMontage)
{
	if (playMontage == nullptr)
		return;

	Montage_Play(playMontage);
}

void UCharacterAnimInst::JumpMontageSection(FName sectionName)
{
	Montage_JumpToSection(sectionName);
}

void UCharacterAnimInst::SetIsStun(bool NewIsStun)
{
	IsStun = NewIsStun;
}