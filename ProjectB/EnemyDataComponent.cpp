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
////// todo : ���� �÷��̾�� ��ų�����͸� ��κ� ��ġ���ѳ����� ���� ���͸� ����ȭ ��Ű�鼭 ���� �ʿ�
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
//	// �̸��� ĳ���� ���׷� ���̺��� �е��� ������ ��
//	SkillData = GameInstance->GetSkillInfo(CharacterName);
//	for (const auto& SD : SkillData)
//	{
//		OwnerCharacter->SkillComponent->AddChainData(SD.PreChainType, SD.ChainType, SD.AxisType, SD.ButtonType, CharacterName, SD.SkillName, SD.ComboStep);
//	}
//	return true;
//}
