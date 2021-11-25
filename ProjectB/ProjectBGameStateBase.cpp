#include "ProjectBGameStateBase.h"

AProjectBGameStateBase::AProjectBGameStateBase()
{
	static ConstructorHelpers::FClassFinder<UUW_WaveDisplay> WB_WaveDisplay(TEXT("WidgetBlueprint'/Game/UI/WB_WaveDisplay.WB_WaveDisplay_C'"));
	if (WB_WaveDisplay.Succeeded())
	{
		WaveDisplayClass = WB_WaveDisplay.Class;
	}

}

void AProjectBGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/*DOREPLIFETIME(AProjectBGameStateBase, CurWaveStep);*/
}

void AProjectBGameStateBase::ResetGame()
{
}

void AProjectBGameStateBase::Multi_AddWaveStep_Implementation(int32 StepNum)
{
	// 이하 UI처리
	if (IsRunningDedicatedServer())
		return;

	if (WaveDisplayObject == nullptr)
	{
		WaveDisplayObject = CreateWidget<UUW_WaveDisplay>(GetWorld()->GetGameInstance(), WaveDisplayClass);
		WaveDisplayObject->AddToViewport();
	}

	if (WaveDisplayObject != nullptr)
	{
		WaveDisplayObject->PlayWaveStartAnim(StepNum);
	}

}
