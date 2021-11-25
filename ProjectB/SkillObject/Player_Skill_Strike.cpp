#include "Player_Skill_Strike.h"
#include <ProjectB/PlayerCharacter.h>
#include <ProjectB/Enemy.h>
#include <DrawDebugHelpers.h>

UPlayer_Skill_Strike::UPlayer_Skill_Strike()
{

}

void UPlayer_Skill_Strike::ExecuteSkill()
{
}

void UPlayer_Skill_Strike::ExecuteSkill(AGameCharacter* Caster)
{
	APlayerCharacter* CastPlayer = Cast<APlayerCharacter>(Caster);
	if (CastPlayer == nullptr)
		return;

	UAnimMontage* mtg = CastPlayer->DataComponent->FindSkillMontage(SkillName);
	if (mtg == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Skill Montage Is Numm : %s"), *SkillName);
		return;
	}

	CastPlayer->AnimInst->PlaySkillMontage(mtg);
	CastPlayer->SetState(ECharacterState::Attack);
}

void UPlayer_Skill_Strike::CustomHitEvent(AGameCharacter* Caster)
{
	APlayerCharacter* CastPlayer = Cast<APlayerCharacter>(Caster);
	if (CastPlayer == nullptr)
		return;

	// todo : 사정거리 등은 Effect에서...
	TArray<AEnemy*> Targets = CheckTargetBySphere(CastPlayer->GetWorld(), CastPlayer, CastPlayer->GetActorLocation(), 500.0f, true);

	FVector CasterForward = CastPlayer->GetActorForwardVector();
	CasterForward.Z = 0.0f;
	CasterForward.Normalize();

	for (const auto& Target : Targets)
	{
		FVector CasterToTarget = Target->GetActorLocation() - CastPlayer->GetActorLocation();
		CasterToTarget.Z = 0.0f;
		CasterToTarget.Normalize();

		float DotAngle = FVector::DotProduct(CasterForward, CasterToTarget);

		if (DotAngle >= 0.1f)
		{
			Target->TakeKnockBack(Caster->GetActorLocation(), 2800.0f);
			
			FTransform SpawnTr;
			SpawnTr.SetLocation(Target->GetActorLocation());
			//SpawnTr.SetScale3D(FVector(0.3f, 0.3f, 0.3f));
			CastPlayer->SpawnImpactEffect(CastPlayer->DataComponent->FindImpactEffect(SkillName), SpawnTr);

			UGameplayStatics::ApplyDamage(Target, 50.0f, nullptr, Caster, UDamageType::StaticClass());
		}
	}
}
