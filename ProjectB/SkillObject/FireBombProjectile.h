#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "FireBombProjectile.generated.h"

UCLASS()
class PROJECTB_API AFireBombProjectile : public AProjectile
{
	GENERATED_BODY()

public:
	AFireBombProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	UParticleSystemComponent* ObjectParticle;
	UPROPERTY()
	UParticleSystem* ImpactParticle;
	UPROPERTY()
	UParticleSystem* ExplosionParticle;
	UPROPERTY()
	USoundCue* ExplosionSound;

	virtual void OverlapEvent(AActor* OverlapActor) override;
	virtual void OverlapEvent(const FHitResult& SweepResult) override;
};
