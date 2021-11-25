#include "GameCharacter.h"
#include <Net/UnrealNetwork.h>
#include <Engine/Engine.h>
#include <Kismet/GameplayStatics.h>

AGameCharacter::AGameCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	MaxHp = 100.0f;
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
	DOREPLIFETIME(AGameCharacter, CurStemina);
	DOREPLIFETIME(AGameCharacter, CharacterName);
}

void AGameCharacter::OnRep_CurHp()
{
}

void AGameCharacter::OnRep_CurStemina()
{
	UE_LOG(LogTemp, Log, TEXT("Cur Stemina : %f"), CurStemina);
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

void AGameCharacter::SetCurStemina(float SteminaValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurStemina = FMath::Clamp(SteminaValue, 0.0f, MaxStemina);
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
