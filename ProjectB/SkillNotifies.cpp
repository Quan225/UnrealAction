#include "SkillNotifies.h"
#include "GameCharacter.h"
#include "PlayerCharacter.h"
#include "Enemy.h"
#include "PlayerCharacterSkillComponent.h"

void UWeaponAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		APlayerCharacter* pc = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (pc != nullptr)
		{
			pc->Weapon->EnableCollider();
		}

		AEnemy* ec = Cast<AEnemy>(MeshComp->GetOwner());
		if (ec != nullptr)
		{
			ec->SetIsWeaponColliderEnable(true);
			ec->SetCurAttackType(AttackType);
		}
	}
}

void UWeaponAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		APlayerCharacter* pc = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (pc != nullptr)
		{
			pc->Weapon->DisableCollider();
		}

		AEnemy* ec = Cast<AEnemy>(MeshComp->GetOwner());
		if (ec != nullptr)
		{
			ec->SetIsWeaponColliderEnable(false);
			ec->SetCurAttackType(AttackType);
		}
	}
}

void UEnableChainInput::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		APlayerCharacter* pc = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (pc != nullptr)
		{
			pc->SkillComponent->SetEnableChainPeriod(true);
		}
	}
}

void UJumpSection::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		APlayerCharacter* pc = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (pc != nullptr)
		{
			if (!pc->SkillComponent->GetEnableChainPeriod())
				return;
			
			pc->SkillComponent->JumpMontageSection();
		}
	}
}

/* 체인 이어짐 방식 변경으로 인한 주석처리
void UJumpSection::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		APlayerCharacter* pc = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (pc != nullptr)
		{
			if (!pc->SkillComponent->GetEnableChainPeriod())
				return;

			pc->SkillComponent->SetEnableChainPeriod(false);
			pc->SkillComponent->JumpMontageSection();
		}
	}
}

void UJumpSection::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		APlayerCharacter* pc = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (pc != nullptr)
		{
		}
	}
}
*/


void UAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		APlayerCharacter* pc = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (pc != nullptr)
		{
			pc->SetState(ECharacterState::Idle);
			pc->SetRegenStaminaDelay();
		}

		AEnemy* ec = Cast<AEnemy>(MeshComp->GetOwner());
		if (ec != nullptr)
		{
			ec->SetState(ECharacterState::Idle);
		}
	}
}

void UExecuteCustomEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		APlayerCharacter* pc = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (pc != nullptr)
		{
			pc->SkillComponent->ExecuteCustomSkillEvent(EventType);
		}

		AEnemy* ec = Cast<AEnemy>(MeshComp->GetOwner());
		if (ec != nullptr)
		{
			ec->SkillComponent->ExecuteCustomSkillEvent(EventType);
		}
	}
}

void UPlaySound_Random::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	int32 RandValue = FMath::RandRange(1, 100);

	if (RandValue <= PlayPercent)
	{
		if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
		{
			AGameCharacter* ch = Cast<AGameCharacter>(MeshComp->GetOwner());
			if (ch != nullptr)
			{
				ch->PlaySoundCue(PlaySound, ch->GetActorLocation());
			}
		}
	}
}
