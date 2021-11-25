#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <DrawDebugHelpers.h>
#include <Kismet/GameplayStatics.h>
#include "GameCharacter.h"
#include "WeaponActor.generated.h"

UCLASS()
class PROJECTB_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void EnableCollider();
	void DisableCollider();
	void Equit(AActor* EquitCharacter);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USkeletalMeshComponent* WeaponMesh;

private:
	bool IsEnableTrace;
	float WeaponPower;

	TArray<AActor*> IgnoreDamageActor;

	void TraceWeaponRange();
};
