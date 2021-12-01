#include "GameCharacter.h"
#include <Net/UnrealNetwork.h>
#include <Engine/Engine.h>
#include <Kismet/GameplayStatics.h>

AGameCharacter::AGameCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	MaxHp = 100.0f;
}

void AGameCharacter::SetMontageDelayPlay(float DelayPlayRate, float DelayPlayLength)
{
	if (GetMesh() == nullptr || GetMesh()->AnimScriptInstance == nullptr)
		return;

	UAnimInstance* AnimInst = GetMesh()->AnimScriptInstance;

	UAnimMontage* CurMontage = AnimInst->GetCurrentActiveMontage();
	if (CurMontage != nullptr)
	{
		AnimInst->Montage_SetPlayRate(CurMontage, DelayPlayRate);
		if (GetWorld()->GetTimerManager().IsTimerActive(MontageDelayTimer))
		{
			GetWorld()->GetTimerManager().ClearTimer(MontageDelayTimer);
		}

		GetWorld()->GetTimerManager().SetTimer(MontageDelayTimer, FTimerDelegate::CreateLambda([CurMontage, AnimInst]()
			{
				if (CurMontage == nullptr)
					return;

				if (CurMontage != AnimInst->GetCurrentActiveMontage())
					return;

				AnimInst->Montage_SetPlayRate(CurMontage, 1.0f);
			}), DelayPlayLength, false);
	}
}

void AGameCharacter::OnRep_IsStun()
{
}

void AGameCharacter::OnRep_IsDead()
{
}

void AGameCharacter::SetIsAttack(bool NewIsAttack)
{
	bIsAttack = NewIsAttack;
}

void AGameCharacter::SetIsStun(bool NewIsStun)
{
	bIsStun = NewIsStun;
}

void AGameCharacter::SetIsDead(bool NewIsDead)
{
	bIsDead = NewIsDead;
}

void AGameCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameCharacter, bIsStun);
	DOREPLIFETIME(AGameCharacter, bIsDead);

	DOREPLIFETIME(AGameCharacter, CurHp);
	DOREPLIFETIME(AGameCharacter, CurStamina);
	DOREPLIFETIME(AGameCharacter, CharacterName);
}

void AGameCharacter::OnRep_CurHp()
{
}

void AGameCharacter::OnRep_CurStamina()
{
	UE_LOG(LogTemp, Log, TEXT("Cur Stamina : %f"), CurStamina);
}

void AGameCharacter::OnRep_CharacterName()
{
}

void AGameCharacter::SetCurHp(float HpValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurHp = FMath::Clamp(HpValue, 0.0f, MaxHp);
	}
}

void AGameCharacter::SetCurStamina(float StaminaValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurStamina = FMath::Clamp(StaminaValue, 0.0f, MaxStamina);
	}
}

ECharacterState AGameCharacter::GetState()
{
	return State;
}

// 오버라이드 해서 사용할 것
void AGameCharacter::InitStatus(FString InitCharacterName)
{
}

void AGameCharacter::SpawnImpactEffect(UParticleSystem* Effect, FTransform Transform)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Effect, Transform);
}

void AGameCharacter::PlaySoundCue(USoundCue* Sound, FVector Location)
{
	if (IsRunningDedicatedServer())
	{
		return;
	}
	
	if (Sound == nullptr)
		return;

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound, Location);
}

void AGameCharacter::TakeKnockBack(FVector CauserPos, float Force)
{
	if (GetLocalRole() < ROLE_Authority)
		return;
	
	FVector launchDir = GetActorLocation() - CauserPos;
	launchDir.Z = 0.0f;
	launchDir.Normalize();
	LaunchCharacter(launchDir * Force, false, false);
}
