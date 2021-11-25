#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "BulletProjectile.generated.h"

UCLASS()
class PROJECTB_API ABulletProjectile : public AProjectile
{
	GENERATED_BODY()
public:
	ABulletProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
		UParticleSystemComponent* ObjectParticle;
	UPROPERTY()
		UParticleSystem* ExplosionParticle;

	virtual void OverlapEvent(AActor* OverlapActor) override;
	virtual void OverlapEvent(const FHitResult& SweepResult) override;
};
