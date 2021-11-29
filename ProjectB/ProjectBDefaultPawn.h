#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include <Engine/Classes/Camera/CameraComponent.h>
#include "ProjectBDefaultPawn.generated.h"

UCLASS()
class PROJECTB_API AProjectBDefaultPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AProjectBDefaultPawn();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
