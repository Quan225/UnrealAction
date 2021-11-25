#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComp"));
	RootComponent = RootSceneComponent;
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::SpawnEnemy(FString EnemyName)
{
	if (EnemyName.IsEmpty())
		return;

	AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(GetActorLocation(), GetActorRotation());
	NewEnemy->Multi_InitCharacter(EnemyName);
}
