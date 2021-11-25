#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include "Projectile.generated.h"

UCLASS()
class PROJECTB_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USphereComponent* OverlapCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY()
		AActor* Target;

public:
	void InitProjectile(AActor* NewTarget, float NewMoveSpeed);
	void SetTarget(AActor* NewTarget);

protected:
	float DetectRange;
	float MoveSpeed;
	void MoveToTarget(float DeltaTime);

protected:
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OverlapEvent(AActor* OverlapActor);
	virtual void OverlapEvent(const FHitResult& SweepResult);
};
