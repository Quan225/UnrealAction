#include "EnemyDataComponent.h"
#include "Enemy.h"

UEnemyDataComponent::UEnemyDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	//LoadSkillData();
}
//
//void UEnemyDataComponent::BeginPlay()
//{
//	Super::BeginPlay();
//}
//
////// todo : 현재 플레이어와 스킬데이터를 대부분 일치시켜놨으나 이후 몬스터를 세분화 시키면서 변경 필요
//bool UEnemyDataComponent::LoadSkillData()
//{
//	if (GameInstance == nullptr)
//	{
//		GameInstance = Cast<UProjectBGameInstance>(GetWorld()->GetGameInstance());
//		if (GameInstance == nullptr)
//		{
//			UE_LOG(LogTemp, Log, TEXT("Instance Null"));
//			return false;
//		}
//	}
//
//	AEnemy* OwnerCharacter = Cast<AEnemy>(GetOwner());
//	if (OwnerCharacter == nullptr)
//	{
//		UE_LOG(LogTemp, Log, TEXT("Owner Null"));
//		return false;
//	}
//
//	// 이름을 캐릭터 제네럴 테이블에서 읽도록 변경할 것
//	SkillData = GameInstance->GetSkillInfo(CharacterName);
//	for (const auto& SD : SkillData)
//	{
//		OwnerCharacter->SkillComponent->AddChainData(SD.PreChainType, SD.ChainType, SD.AxisType, SD.ButtonType, CharacterName, SD.SkillName, SD.ComboStep);
//	}
//	return true;
//}
