#include "Enemy_Dusk_Skill_Fire.h"
#include <ProjectB/Enemy.h>
#include <ProjectB/PlayerCharacter.h>
#include "BulletProjectile.h"
#include <DrawDebugHelpers.h>

UEnemy_Dusk_Skill_Fire::UEnemy_Dusk_Skill_Fire()
{
}

void UEnemy_Dusk_Skill_Fire::ExecuteSkill()
{
}

void UEnemy_Dusk_Skill_Fire::ExecuteSkill(AGameCharacter* Caster)
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

void UEnemy_Dusk_Skill_Fire::CustomSpawnEvent(AGameCharacter* Caster)
{
	AEnemy* CastEnemy = Cast<AEnemy>(Caster);
	if (CastEnemy == nullptr)
		return;

	FVector MuzleLocation = Caster->GetMesh()->GetSocketLocation("Muzzle_Front");

	SpawnProjectile = CastEnemy->GetWorld()->SpawnActor<ABulletProjectile>(MuzleLocation, CastEnemy->GetActorRotation());
	//SpawnProjectile->InitProjectile(nullptr, 2500.0f);


	//// todo : �����鸮 ���̾� ���ϱ� �����Ƽ� �̷��� �ص״µ�, ���߿� ���ݿ� �±� ���� �ٴ� ���� ���� ��
	//TArray<AEnemy*> Targets = CheckTargetBySphere(CastPlayer->GetWorld(), CastPlayer, CastPlayer->GetActorLocation(), 500.0f, true);

	//FVector CasterForward = CastPlayer->GetActorForwardVector();
	//CasterForward.Z = 0.0f;
	//CasterForward.Normalize();

	//for (const auto& Target : Targets)
	//{
	//	FVector CasterToTarget = Target->GetActorLocation() - CastPlayer->GetActorLocation();
	//	CasterToTarget.Z = 0.0f;
	//	CasterToTarget.Normalize();

	//	float DotAngle = FVector::DotProduct(CasterForward, CasterToTarget);

	//	if (DotAngle >= 0.1f)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("HIT"));
	//		Target->TakeKnockBack(Caster->GetActorLocation(), 1800.0f);
	//		UGameplayStatics::ApplyDamage(Target, 50.0f, nullptr, Caster, UDamageType::StaticClass());
	//	}
	//}
}
