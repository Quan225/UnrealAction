#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.h"
#include <Components/CapsuleComponent.h>
#include <Components/SphereComponent.h>
#include "ItemActor.generated.h"

UCLASS()
class PROJECTB_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* OverlapCollider;

protected:
	UPROPERTY()
	UItem* ItemData;

	UPROPERTY(Replicated)
	int32 ItemIdx;
	UPROPERTY(Replicated)
	int32 ItemCount;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// [server]
	UFUNCTION()
	bool Server_PickUp(AActor* OverlapActor);
	// [client]
	UFUNCTION(Client, Reliable)
	void Client_PickUp(AActor* OverlapActor);

public:
	void InitItemData(int32 NewItemIdx);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
