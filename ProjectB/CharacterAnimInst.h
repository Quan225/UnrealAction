#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include <Engine/Classes/Animation/AnimSequence.h>
#include <Engine/Classes/Animation/AnimMontage.h>
#include "Engine/DataTable.h"
#include "AnimType.h"
#include "CharacterAnimInst.generated.h"


UCLASS()
class PROJECTB_API UCharacterAnimInst : public UAnimInstance
{
	GENERATED_BODY()
public:
	UCharacterAnimInst();
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void StopAllMontage();
	void PlaySkillMontage(UAnimMontage* playMontage);
	void JumpMontageSection(FName sectionName);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MoveSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float IsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsStun;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsDodge;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float DodgeDegree;
public:
	void SetMoveSpeed(float NewSpeed) { MoveSpeed = NewSpeed; }
	void SetIsInAir(float NewIsInAir) { MoveSpeed = NewIsInAir; }
	void SetIsAttack(bool NewIsAttack) { IsAttack = NewIsAttack; }
	void SetIsStun(bool NewIsStun);
	void SetIsDead(bool NewIsDead) { IsDead = NewIsDead; }
	void SetIsDodge(bool NewIsDodge) { IsDodge = NewIsDodge; }
	void SetDodgeDegree(float NewDodgeDegree) { DodgeDegree = NewDodgeDegree; }

};
