#pragma once

#include "CoreMinimal.h"
#include "GameCharacter.h"
#include <Engine/Classes/GameFramework/SpringArmComponent.h>
#include <Engine/Classes/Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "WeaponActor.h"
#include "PlayerAnimData.h"
#include "PlayerAnimInst.h"
#include "PlayerDataComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Item.h"
#include "PlayerCharacterSkillComponent.h"
#include "PlayerCharacter.generated.h"

enum class EAxisInputType : uint8;
enum class EButtonInputType : uint8;

UCLASS()
class PROJECTB_API APlayerCharacter : public AGameCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AWeaponActor* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPlayerDataComponent* DataComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPlayerCharacterSkillComponent* SkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPlayerAnimInst* AnimInst;

private:
	void InitStatus(FString InitCharacterName);

protected:
	virtual void OnRep_CurHp() override;
	virtual void OnRep_IsStun() override;
	virtual void OnRep_IsDead() override;

public:
	// 플레이어 상태 관리
	virtual bool SetState(ECharacterState NewState) override;
	virtual void Dead() override;
	virtual bool Stun() override;

protected:
	bool CanAttack();

public:
	// 플레이어 이동 관련 함수
	void StartJump();
	void EndJump();
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	virtual void AddControllerPitchInput(float Val);
	virtual void AddControllerYawInput(float Val);

	void RotateActorToFront(bool bIsReadKey);

private:
	// 플레이어 회피 관련
	bool bIsInvisible;
	FVector DirDodge;

	bool Dodge(EAxisInputType AxisType);

private:
	// 인벤토리 관련
	bool bIsInventoryOpen;
	int32 InventoryCapacity;

	void ToggleInventory();

public:
	int32 GetInventoryCapacity() { return InventoryCapacity; }

private:
	// 플레이어 공격 관련

	EAxisInputType ReadAxisInputState();
	EButtonInputType CurButtonState;

public:
	void ResetAttackFlags();

protected:

	UFUNCTION(Server, Reliable)
	void Server_UseSkill(EAxisInputType AxisType, EButtonInputType ButtonType);
	UFUNCTION(NetMulticast, Reliable)
	void Multi_UseSkill(EAxisInputType AxisType, EButtonInputType ButtonType);
	void UseSkill(EAxisInputType AxisType, EButtonInputType ButtonType);

	void AttackRight();
	void PresseSkillQ();
	void ReleaseSkillQ();

	UFUNCTION(Server, Reliable)
	void ServerTestMouseR();
	UFUNCTION(NetMulticast, Reliable)
	void Multi_TestMouseR();
	void TestMouseR();

public:
	UFUNCTION(Client, Reliable)
	void Client_PickUpItem(int32 PickItemData, int32 PickAddCount);

public:
	virtual float TakeDamage(float DamageAMount, FDamageEvent const& DamageEnvent, AController* EventInstigator, AActor* DamageCauser) override;

};

