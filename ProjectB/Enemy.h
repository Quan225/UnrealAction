#pragma once

#include "CoreMinimal.h"
#include "GameCharacter.h"
#include "EnemyAnimInst.h"
#include "EnemyAIController.h"
#include "EnemyDataComponent.h"
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>
#include "EnemySkillComponent.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyWeaponRangeType : uint8
{
	None = 0,
	LeftSword = 1,
	RightSword = 2,
	TwinSword = 3
};

UCLASS()
class PROJECTB_API AEnemy : public AGameCharacter
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void PostInitializeComponents() override;

	//UPROPERTY(EditAnywhere, Category = Behavior)
	//class UBehaviorTree* EnemyBehaviorTree;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAMount, FDamageEvent const& DamageEnvent, AController* EventInstigator, AActor* DamageCauser) override;
public:
	//[server + client]
	UFUNCTION(NetMulticast, Reliable)
	void Multi_InitCharacter(const FString& InitCharacterName);
protected:
	//[server]
	virtual void InitStatus(FString InitCharacterName) override;
	
protected:
	// 애니메이션 관련
	virtual void OnRep_IsStun() override;
	virtual void OnRep_IsDead() override;
	virtual void OnRep_CharacterName() override;

public:
	virtual bool SetState(ECharacterState NewState) override;
	virtual bool Stun() override;
	virtual void Dead() override;

	UFUNCTION(NetMulticast, Reliable)
	void Multi_Attack();
	// [Server + Client]
	UFUNCTION()
	void Attack();
	bool CanAttack();
	void ResetAttackFlag();

	void TraceWeaponRange(EEnemyWeaponRangeType RangeType);
	void TraceWeaponRange(FVector Start, FVector End);

private:
	bool IsWeaponColliderEnable;
	EEnemyWeaponRangeType CurAttackType;
	TArray<AActor*> IgnoreDamageActor;

public:
	void SetIsWeaponColliderEnable(bool IsEnable);
	void SetCurAttackType(EEnemyWeaponRangeType AttackType) { CurAttackType = AttackType; }

private:
	void UpdateMaterialAppearance(float DeltaTime);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterAnimInst* AnimInst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEnemyDataComponent* DataComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UEnemySkillComponent* SkillComponent;

	UPROPERTY()
	UMaterialInstanceDynamic* MaterialDynamic;
	float MatAppearance;
	bool IsAppearance;

	// 임시빌드용 코드
	TSubclassOf<UAnimInstance> GunAnimClass;
	TSubclassOf<UAnimInstance> SwordAnimClass;
};
