#include "Player_Skill_Shield_Throw.h"
#include <ProjectB/PlayerCharacter.h>
#include <ProjectB/Enemy.h>
#include "FireBombProjectile.h"

UPlayer_Skill_Shield_Throw::UPlayer_Skill_Shield_Throw()
{

}

void UPlayer_Skill_Shield_Throw::ExecuteSkill()
{
}

void UPlayer_Skill_Shield_Throw::ExecuteSkill(AGameCharacter* Caster)
{
	APlayerCharacter* CastPlayer = Cast<APlayerCharacter>(Caster);
	if (CastPlayer == nullptr)
		return;

	UAnimMontage* mtg = CastPlayer->DataComponent->FindSkillMontage(SkillName);
	if (mtg == nullptr)
		return;

	CastPlayer->AnimInst->PlaySkillMontage(mtg);
	CastPlayer->SetState(ECharacterState::Attack);


	TArray<AEnemy*> DetectedTargets = CheckTargetBySphere(CastPlayer->GetWorld(), CastPlayer, CastPlayer->GetActorLocation(), 1500.0f, true);
	if (DetectedTargets.Num() <= 0)
		return;

	FVector CasterForward = CastPlayer->GetActorForwardVector();
	CasterForward.Z = 0.0f;
	CasterForward.Normalize();

	float MinDistance = -100.0f;
	int MinIdx = 0;

	for (int32 i = 0; i < DetectedTargets.Num(); i++)
	{
		FVector CasterToTarget = DetectedTargets[i]->GetActorLocation() - CastPlayer->GetActorLocation();
		CasterToTarget.Z = 0.0f;
		CasterToTarget.Normalize();

		float DotAngle = FVector::DotProduct(CasterForward, CasterToTarget);

		if (DotAngle >= 0.1f)
		{
			if (MinDistance < 0 || MinDistance > CasterToTarget.Size())
			{
				MinDistance = CasterToTarget.Size();
				MinIdx = 0;
			}
		}
	}

	Target = DetectedTargets[MinIdx];
}

void UPlayer_Skill_Shield_Throw::CustomSpawnEvent(AGameCharacter* Caster)
{
	//if (Target == nullptr)
	//	return;

	APlayerCharacter* CastPlayer = Cast<APlayerCharacter>(Caster);
	if (CastPlayer == nullptr)
		return;

	SpawnProjectile = CastPlayer->GetWorld()->SpawnActor<AFireBombProjectile>(CastPlayer->GetActorLocation(), CastPlayer->GetActorRotation());
	SpawnProjectile->InitProjectile(Target, 1400.0f);
}