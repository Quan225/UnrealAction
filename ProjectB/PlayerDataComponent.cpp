#include "PlayerDataComponent.h"
#include "PlayerCharacter.h"

UPlayerDataComponent::UPlayerDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

//void UPlayerDataComponent::BeginPlay()
//{
//	Super::BeginPlay();
//}
//
//bool UPlayerDataComponent::LoadSkillData()
//{
//	if (GameInstance == nullptr)
//	{
//		return false;
//	}
//
//	APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(GetOwner());
//	if (OwnerCharacter == nullptr)
//		return false;
//
//	// 이름을 캐릭터 제네럴 테이블에서 읽도록 변경할 것
//	SkillData = GameInstance->GetSkillInfo(CharacterName);
//	for (const auto& SD : SkillData)
//	{
//		OwnerCharacter->SkillComponent->AddChainData(SD.PreChainType, SD.ChainType, SD.AxisType, SD.ButtonType, CharacterName, SD.SkillName, SD.ComboStep);
//	}
//
//	return true;
//}
