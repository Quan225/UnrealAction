#include "ProjectBDefaultPawn.h"

AProjectBDefaultPawn::AProjectBDefaultPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

void AProjectBDefaultPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectBDefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectBDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

