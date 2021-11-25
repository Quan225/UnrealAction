#include "FireBombProjectile.h"
#include <Particles/ParticleSystemComponent.h>
#include <Kismet/GameplayStatics.h>
#include <ProjectB/GameCharacter.h>
#include <Kismet/KismetMathLibrary.h>

AFireBombProjectile::AFireBombProjectile()
{
	Mesh->SetSkeletalMesh(nullptr);


	ObjectParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ObjectParticle"));
	ConstructorHelpers::FObjectFinder<UParticleSystem> ObjPtc(TEXT("ParticleSystem'/Game/ParagonGRIMexe/FX/Particles/Abilities/BFG/FX/P_GRIM_BFG_Projectile_New.P_GRIM_BFG_Projectile_New'"));
	if (ObjPtc.Succeeded())
	{
		ObjectParticle->SetTemplate(ObjPtc.Object);
	}
	ObjectParticle->AttachTo(RootComponent);
	ObjectParticle->bAutoActivate = true;
	ObjectParticle->bAutoDestroy = false;
	ObjectParticle->SetRelativeScale3D(FVector(0.4f, 0.4f, 0.4f));

	ConstructorHelpers::FObjectFinder<UParticleSystem> ImpPtc(TEXT("ParticleSystem'/Game/ParagonGRIMexe/FX/Particles/Abilities/Ultimate/FX/P_GRIM_Ult_ChestBlast.P_GRIM_Ult_ChestBlast'"));
	if (ImpPtc.Succeeded())
	{
		ImpactParticle = ImpPtc.Object;
	}
	ConstructorHelpers::FObjectFinder<UParticleSystem> ExpPtc(TEXT("ParticleSystem'/Game/ParagonGRIMexe/FX/Particles/Abilities/BFG/FX/P_GRIM_BFG_Explosion.P_GRIM_BFG_Explosion'"));
	if (ExpPtc.Succeeded())
	{
		ExplosionParticle = ExpPtc.Object;
	}
	ConstructorHelpers::FObjectFinder<USoundCue> ExpSound(TEXT("SoundCue'/Game/UsingSounds/PlayerCharacter/Projectile_FireBomb_Explosion.Projectile_FireBomb_Explosion'"));
	if (ExpPtc.Succeeded())
	{
		ExplosionSound = ExpSound.Object;
	}
}

void AFireBombProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AFireBombProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireBombProjectile::OverlapEvent(AActor* OverlapActor)
{
	
}

void AFireBombProjectile::OverlapEvent(const FHitResult& SweepResult)
{
	if (IsPendingKill())
		return;

	TArray<AActor*> DamagedActors;
	UGameplayStatics::ApplyRadialDamage_V2(GetWorld(), 20.0f, SweepResult.ImpactPoint, 300.0f, nullptr, TArray<AActor*>(), DamagedActors);
	
	FTransform ExpSpawnTr;
	ExpSpawnTr.SetLocation(SweepResult.ImpactPoint);
	ExpSpawnTr.SetScale3D(FVector(0.5f, 0.5f, 0.5f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, ExpSpawnTr);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());

	for (int32 i = 0; i < DamagedActors.Num(); i++)
	{
		AGameCharacter* DA = Cast<AGameCharacter>(DamagedActors[i]);
		if (DA == nullptr)
			continue;

		FTransform ImpSpawnTr;
		ImpSpawnTr.SetLocation(DA->GetActorLocation());
		ImpSpawnTr.SetRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DA->GetActorLocation()).Quaternion());
		DA->SpawnImpactEffect(ImpactParticle, ImpSpawnTr);
	}

	Destroy();
}
