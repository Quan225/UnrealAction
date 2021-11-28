#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "DataComponent.h"
#include "GameFramework/Character.h"
#include "GameCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	None = 0,
	Idle = 1,
	Attack = 2,
	Stun = 3,
	Dodge = 4,
	Dead = 99
};

UCLASS()
class PROJECTB_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGameCharacter();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMeshComponent* CharacterMesh;

protected:
	// 애니메이션 관련
	UPROPERTY(Transient)
	float bIsInAir;
	UPROPERTY(Transient)
	bool bIsAttack; // ExecuteSkill을 통해 Replicate
	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsStun)
	bool bIsStun;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsDead)
	bool bIsDead;
	bool bIsDodge;
	float DodgeDegree;

	UFUNCTION()
	virtual void OnRep_IsStun();
	UFUNCTION()
	virtual void OnRep_IsDead();

public:
	void SetIsAttack(bool NewIsAttack);
	void SetIsStun(bool NewIsStun);
	void SetIsDead(bool NewIsDead);

protected:
	// 스탯
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Transient, ReplicatedUsing=OnRep_CurHp)
		float CurHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, ReplicatedUsing=OnRep_CurStamina)
		float CurStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float IsDead;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CharacterName)
		FString CharacterName;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	virtual void OnRep_CurHp();
	UFUNCTION()
	void OnRep_CurStamina();
	UFUNCTION()
	virtual void OnRep_CharacterName();

public:
	void SetCurHp(float HpValue);
	virtual void SetCurStamina(float StaminaValue);

public:
	float GetMaxHp() { return MaxHp; }
	float GetCurHp() { return CurHp; }
	float GetMaxStamina() { return MaxStamina; }
	float GetCurStamina() { return CurStamina; }

public:
	ECharacterState State;
	ECharacterState GetState();
	virtual bool SetState(ECharacterState NewState) { return false; };
	virtual void InitStatus(FString InitCharacterName);

public:
	void SpawnImpactEffect(UParticleSystem* Effect, FTransform Transform);
	void PlaySoundCue(USoundCue* Sound, FVector Location);
	void TakeKnockBack(FVector CauserPos, float Force);

protected:
	virtual void Dead() {};
	virtual bool Stun() { return true; };
};