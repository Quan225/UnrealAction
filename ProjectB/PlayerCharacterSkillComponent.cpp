#include "PlayerCharacterSkillComponent.h"
#include "PlayerCharacter.h"

UPlayerCharacterSkillComponent::UPlayerCharacterSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerCharacterSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayerCharacter = Cast<APlayerCharacter>(OwnerCharacter);
	CurChainStep = 0;
}

bool UPlayerCharacterSkillComponent::ChainOtherSkill()
{
	UAnimMontage* curMontage = OwnerPlayerCharacter->AnimInst->GetCurrentActiveMontage();
	OwnerPlayerCharacter->AnimInst->Montage_Stop(0.5, curMontage);

	if (!ChainData.Contains(OtherChainType))
		return false;

	for (int32 i = 0; i < ChainData[OtherChainType].Sequences.Num(); i++)
	{
		if (ChainData[OtherChainType].Sequences[i].ChainStep != CurChainStep)
			continue;
		if (ChainData[OtherChainType].Sequences[i].SkillName != NextChainSkillName)
			continue;

		ChainData[OtherChainType].Sequences[i].BindSkillObject->ExecuteSkill(OwnerCharacter);
		CurChainType = ChainData[OtherChainType].Sequences[i].ChainType;
		return true;
	}
	return false;
}

bool UPlayerCharacterSkillComponent::UseSkill(EAxisInputType AxisInput, EButtonInputType ButtonInput)
{
	FSkillSequence* NextSequence = FindChainSkillSequence(AxisInput, ButtonInput);
	if (NextSequence == nullptr)
		return false;

	if (CurChainStep == 0)
	{
		CurSkillObject = NextSequence->BindSkillObject;
		CurChainType = NextSequence->ChainType;
		NextChainSkillName = NextSequence->SkillName;
		NextSequence->BindSkillObject->ExecuteSkill(OwnerPlayerCharacter);
		
		CurChainStep++;
		return true;
	}
	else
	{
		if (!NextChainSkillName.Equals(NextSequence->SkillName))
		{
			bIsChainOtherSkill = true;
			OtherChainType = CurChainType;
		}
		
		CurSkillObject = NextSequence->BindSkillObject;
		CurChainType = NextSequence->ChainType;
		NextChainSkillName = NextSequence->SkillName;
		
		bIsChainNext = true;
		return true;
	}

	return false;
}

FSkillSequence* UPlayerCharacterSkillComponent::FindChainSkillSequence(EAxisInputType AxisInput, EButtonInputType ButtonInput)
{
	// 현재 상태에서 연결되는 동작이 있는지 확인
	if (!ChainData.Contains(CurChainType))
	{
		UE_LOG(LogTemp, Log, TEXT("Next Chain Skill Not Found"));
		return nullptr;
	}

	// [server] 체인 입력이 가능한지 확인
	if (CurChainStep != 0 && !bEnableChainPeriod)
	{
		UE_LOG(LogTemp, Log, TEXT("Not Enable Chain Period"));
		return nullptr;
	}

	// 이미 다음 동작이 정해졌는지 확인
	if (bIsChainNext)
	{
		UE_LOG(LogTemp, Log, TEXT("Already Set Next Skill"));
		return nullptr;
	}

	// 커맨드에 정확히 일치하는 스킬을 탐색
	for (int32 i = 0; i < ChainData[CurChainType].Sequences.Num(); i++)
	{
		if (ChainData[CurChainType].Sequences[i].AxisInputType != AxisInput)
			continue;
		if (ChainData[CurChainType].Sequences[i].ButtonInputType != ButtonInput)
			continue;

		return &ChainData[CurChainType].Sequences[i];
	}

	// 커맨드에 해당하는 스킬이 없음 && Axis이 None인 스킬 중 버튼이 일치
	for (int32 i = 0; i < ChainData[CurChainType].Sequences.Num(); i++)
	{
		if (ChainData[CurChainType].Sequences[i].AxisInputType != EAxisInputType::None)
			continue;
		if (ChainData[CurChainType].Sequences[i].ButtonInputType != ButtonInput)
			continue;

		return &ChainData[CurChainType].Sequences[i];
	}
	

	return nullptr;
}

void UPlayerCharacterSkillComponent::ResetChainFlags()
{
	Super::ResetChainFlags();

	OtherChainType = EChainType::None;
	CurChainStep = 0;
	bIsChainNext = false;
	bEnableChainPeriod = false;
	bIsChainOtherSkill = false;
}

void UPlayerCharacterSkillComponent::JumpMontageSection()
{
	if (!bIsChainNext)
		return;

	OwnerPlayerCharacter->RotateActorToFront(true);

	bIsChainNext = false;
	bEnableChainPeriod = false;
	CurChainStep++;

	if (bIsChainOtherSkill)
	{
		bIsChainOtherSkill = false;
		if (!ChainOtherSkill())
			return;
	}
	FName NextSectionName = OwnerPlayerCharacter->DataComponent->FindSkillMontageSection(NextChainSkillName, CurChainStep);
	OwnerPlayerCharacter->AnimInst->JumpMontageSection(NextSectionName);
}