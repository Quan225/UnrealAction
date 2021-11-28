#include "CharacterSkillComponent.h"
#include "GameCharacter.h"
#include "PlayerCharacter.h"
#include "Enemy.h"
#include <ProjectB/ProjectBGameInstance.h>
#include "SkillObject/SkillObject.h"

UCharacterSkillComponent::UCharacterSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AGameCharacter>(GetOwner());
	CurChainType = EChainType::None;
	CurSkillObject = nullptr;
}

void UCharacterSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterSkillComponent::AddChainData(EChainType PreChainType, EChainType ChainType, EAxisInputType AxisType, EButtonInputType ButtonType, float RequireStamina, FString CharacterName, FString SkillName, int32 ChainStep)
{
	UProjectBGameInstance* inst = Cast<UProjectBGameInstance>(GetWorld()->GetGameInstance());
	if (inst == nullptr)
		return;
	
	USkillObject* skillObj = inst->FindSkillObject(CharacterName, SkillName, ChainStep);
	if (skillObj == nullptr)
		return;

	FSkillSequence newSequence;
	newSequence.AxisInputType = AxisType;
	newSequence.ButtonInputType = ButtonType;
	newSequence.BindSkillObject = skillObj;
	newSequence.SkillName = SkillName;
	newSequence.ChainStep = ChainStep;
	newSequence.ChainType = ChainType;
	newSequence.RequireStamina = RequireStamina;

	if (ChainData.Contains(PreChainType) && ChainData[PreChainType].Sequences.Num() > 0)
	{
		for (const auto& se : ChainData[PreChainType].Sequences)
		{
			if (se.AxisInputType != newSequence.AxisInputType || se.ButtonInputType != newSequence.ButtonInputType)
			{
				ChainData[PreChainType].Sequences.Add(newSequence);
				return;
			}
		}
	}
	else
	{
		if (ChainData.Contains(PreChainType))
		{
			ChainData[PreChainType].Sequences.Add(newSequence);
		}
		else
		{

			FSkillSequenceArray sequenceArray;
			sequenceArray.Sequences.Add(newSequence);
			ChainData.Add(PreChainType, sequenceArray);
		}
	}
}

bool UCharacterSkillComponent::UseSkill(EAxisInputType AxisInput, EButtonInputType ButtonInput)
{
	UProjectBGameInstance* inst = Cast<UProjectBGameInstance>(GetWorld()->GetGameInstance());
	if (inst == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("UseSkill : Instance is Null"));
		return false;
	}

	if (!ChainData.Contains(CurChainType))
	{
		UE_LOG(LogTemp, Log, TEXT("UseSkill : ChainData not contains"));
		return false;
	}

	if (AxisInput != EAxisInputType::None)
	{
		for (int32 i = 0; i < ChainData[CurChainType].Sequences.Num(); i++)
		{
			if (ChainData[CurChainType].Sequences[i].AxisInputType != AxisInput)
				continue;
			if (ChainData[CurChainType].Sequences[i].ButtonInputType != ButtonInput)
				continue;

			CurSkillObject = ChainData[CurChainType].Sequences[i].BindSkillObject;
			ChainData[CurChainType].Sequences[i].BindSkillObject->ExecuteSkill(OwnerCharacter);
			NextChainSkillName = ChainData[CurChainType].Sequences[i].SkillName;
			CurChainType = ChainData[CurChainType].Sequences[i].ChainType;
			return true;
		}
	}

	for (int32 i = 0; i < ChainData[CurChainType].Sequences.Num(); i++)
	{
		if (ChainData[CurChainType].Sequences[i].ButtonInputType != ButtonInput)
			continue;

		CurSkillObject = ChainData[CurChainType].Sequences[i].BindSkillObject;
		ChainData[CurChainType].Sequences[i].BindSkillObject->ExecuteSkill(OwnerCharacter);
		NextChainSkillName = ChainData[CurChainType].Sequences[i].SkillName;
		CurChainType = ChainData[CurChainType].Sequences[i].ChainType;
		return true;
	}

	UE_LOG(LogTemp, Log, TEXT("UseSkill : ChainData not find"));
	return false;
}

void UCharacterSkillComponent::ResetChainFlags()
{
	CurChainType = EChainType::None;
	NextChainSkillName = "";
	CurSkillObject = nullptr;
}

void UCharacterSkillComponent::ExecuteCustomSkillEvent(ENotifySkillType EventType)
{
	if (OwnerCharacter == nullptr)
		return;

	if (GetOwnerRole() < ROLE_Authority)
		return;

	if (CurSkillObject == nullptr)
		return;

	if (EventType == ENotifySkillType::HitSphere)
	{
		CurSkillObject->CustomHitEvent(OwnerCharacter);
	}
	else if (EventType == ENotifySkillType::SpawnActor)
	{
		CurSkillObject->CustomSpawnEvent(OwnerCharacter);
	}
	else
	{
		return;
	}
}
