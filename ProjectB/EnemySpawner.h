#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "EnemySpawner.generated.h"

UCLASS()
class PROJECTB_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* RootSceneComponent;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void SpawnEnemy(FString EnemyName);

};
