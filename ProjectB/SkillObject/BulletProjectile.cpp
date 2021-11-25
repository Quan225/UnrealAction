#include "BulletProjectile.h"
#include <Particles/ParticleSystemComponent.h>
#include <Kismet/GameplayStatics.h>
#include <ProjectB/PlayerCharacter.h>
#include <Kismet/KismetMathLibrary.h>
#include <GameFramework/ProjectileMovementComponent.h>

ABulletProjectile::ABulletProjectile()
{
	Mesh->SetSkeletalMesh(nullptr);

	OverlapCollider->SetCollisionProfileName("SpawnByEnemy");

	ObjectParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ObjectParticle"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> ObjPtc(TEXT("ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_Projectile.P_Wraith_Sniper_Projectile'"));
	if (ObjPtc.Succeeded())
	{
		ObjectParticle->SetTemplate(ObjPtc.Object);
	}
	ObjectParticle->AttachTo(RootComponent);
	ObjectParticle->bAutoActivate = true;
	ObjectParticle->bAutoDestroy = false;
	ObjectParticle->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));

	ConstructorHelpers::FObjectFinder<UParticleSystem> ExpPtc(TEXT("ParticleSystem'/Game/ParagonWraith/FX/Particles/Abilities/ScopedShot/FX/P_Wraith_Sniper_HitCharacter.P_Wraith_Sniper_HitCharacter'"));
	if (ExpPtc.Succeeded())
	{
		ExplosionParticle = ExpPtc.Object;
	}


	ProjectileMovementComponent->InitialSpeed = 2500.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ABulletProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABulletProjectile::OverlapEvent(AActor* OverlapActor)
{
	if (IsPendingKill())
		return;

	FTransform ExpSpawnTr;
	ExpSpawnTr.SetLocation(GetActorLocation());
	ExpSpawnTr.SetScale3D(FVector(0.5f, 0.5f, 0.5f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, ExpSpawnTr);

	Destroy();

	if (!OverlapActor->IsA(APlayerCharacter::StaticClass()))
		return;

	UGameplayStatics::ApplyDamage(OverlapActor, 15.0f, nullptr, this, UDamageType::StaticClass());
}

void ABulletProjectile::OverlapEvent(const FHitResult& SweepResult)
{
	
}
