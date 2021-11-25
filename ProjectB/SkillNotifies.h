#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SkillNotifies.generated.h"

UCLASS()
class PROJECTB_API UPlaySound_Random : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USoundCue* PlaySound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 PlayPercent; // 0 ~ 100;
};

UCLASS()
class PROJECTB_API UWeaponAttack : public UAnimNotifyState
{
	GENERATED_BODY()

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyWeaponRangeType AttackType;
};

UCLASS()
class PROJECTB_API UEnableChainInput : public UAnimNotify
{
	GENERATED_BODY()

		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS()
class PROJECTB_API UJumpSection : public UAnimNotifyState
{
	GENERATED_BODY()

	//virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;
	//virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS()
class PROJECTB_API UAttackEnd : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

UCLASS()
class PROJECTB_API UExecuteCustomEvent : public UAnimNotify
{
	GENERATED_BODY()

		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		ENotifySkillType EventType;
};
