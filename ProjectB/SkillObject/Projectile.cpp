#include "Projectile.h"
#include <ProjectB/Enemy.h>
#include <GameFramework/ProjectileMovementComponent.h>

AProjectile::AProjectile()
{
 	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;


	OverlapCollider = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapCollider->SetCollisionProfileName("SpawnByPlayer");
	OverlapCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	RootComponent = OverlapCollider;
	//OverlapCollider->AttachTo(RootComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->AttachTo(RootComponent);
	Mesh->SetRelativeRotation(FRotator(0, -90.0f, 0));
	//RootComponent = Mesh;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> LoadMesh(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Buff/Buff_Blue/Meshes/Buff_Blue.Buff_Blue'"));
		//static ConstructorHelpers::FObjectFinder<USkeletalMesh> LoadMesh(TEXT("SkeletalMesh'/Game/Blade_HeroSword11/SK_Blade_HeroSword11.SK_Blade_HeroSword11'"));
	if (LoadMesh.Succeeded())
	{
		Mesh->SetSkeletalMesh(LoadMesh.Object);
		//Mesh->SetWorldRotation(FRotator(90.0f, 0.0f, 90.0f));
	}

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovmentComp"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);
	ProjectileMovementComponent->InitialSpeed = 1300.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.7f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	DetectRange = 800.0f;
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//MoveToTarget(DeltaTime);
}

void AProjectile::InitProjectile(AActor* NewTarget, float NewMoveSpeed)
{
	Target = NewTarget;
	MoveSpeed = NewMoveSpeed;
}

void AProjectile::SetTarget(AActor* NewTarget)
{
	Target = NewTarget;
}

void AProjectile::MoveToTarget(float DeltaTime)
{
	if (Target == nullptr)
		return;

	FVector ToTargetDir = Target->GetActorLocation() - GetActorLocation();
	ToTargetDir.Normalize();

	FVector Location = GetActorLocation();
	Location += ToTargetDir * MoveSpeed * DeltaTime;
	SetActorLocation(Location);

}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapEvent(OtherActor);
	OverlapEvent(SweepResult);
}

void AProjectile::OverlapEvent(AActor* OverlapActor)
{
	UE_LOG(LogTemp, Log, TEXT("Projectile Overlap"));
	AEnemy* Enemy = Cast<AEnemy>(OverlapActor);
	if (Enemy == nullptr)
		return;

	UGameplayStatics::ApplyDamage(Enemy, 5.0f, nullptr, this, UDamageType::StaticClass());

	TArray<FOverlapResult> overlapResults;

	//FCollisionQueryParams collParam(NAME_None, false);
	//collParam.bFindInitialOverlaps = true;

	//bool isOverlap = GetWorld()->OverlapMultiByChannel(
	//	overlapResults,
	//	GetActorLocation(),
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel2,
	//	FCollisionShape::MakeSphere(DetectRange),
	//	collParam
	//);

	//if (isOverlap)
	//{
	//	for (const auto& result : overlapResults)
	//	{
	//		AEnemy* HitEnemy = Cast<AEnemy>(result.GetActor());
	//		if (HitEnemy == nullptr)
	//			continue;
	//		
	//		if (HitEnemy == Target)
	//			continue;

	//		SetTarget(HitEnemy);
	//		DrawDebugSphere(GetWorld(), GetActorLocation(), DetectRange, 16, FColor::Black, false, 0.2f);
	//		return;
	//	}
	//}
	Destroy();
}

void AProjectile::OverlapEvent(const FHitResult& SweepResult)
{
}

