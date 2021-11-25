#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "UI/UW_WaveDisplay.h"
#include "ProjectBGameStateBase.generated.h"

UCLASS()
class PROJECTB_API AProjectBGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AProjectBGameStateBase();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void ResetGame();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_AddWaveStep(int32 StepNum);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUW_WaveDisplay> WaveDisplayClass;
	UPROPERTY()
	class UUW_WaveDisplay* WaveDisplayObject;
};
