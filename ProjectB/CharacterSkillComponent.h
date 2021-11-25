#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <ProjectB/ProjectBGameInstance.h>
#include "CharacterSkillComponent.generated.h"

UENUM(BlueprintType)
enum class EAxisInputType : uint8
{
	// ��ų�� �ߵ���Ű�� ���� �䱸�Ǵ� ���� �߰��Է�
	None = 0,
	Front = 1,
	Back = 2,
	Right = 3,
	Left = 4
};

UENUM(BlueprintType)
enum class EButtonInputType : uint8
{
	// ��ų�� �ߵ���Ű�� ���� �䱸�Ǵ� ��ư �߰��Է�
	None = 0,
	Skill_Q = 1,
	Space = 2,
};

UENUM(BlueprintType)
enum class EChainType : uint8
{
	// ��ų�� �ߵ���ų �� �ִ� �ܰ�
	None = 0, // ��� ��밡��
	Basic_1 = 1, // Basic_1 Ÿ���� ��ų �ߵ� �� �Ļ� ����
	Basic_2 = 2,
	Basic_3 = 3,
	Basic_4 = 4,
	Skill_Q = 5,
	Skill_R = 6
};

UENUM(BlueprintType)
enum class ENotifySkillType : uint8
{
	// ��ų ��Ƽ���̿��� �߻���ų ��ų�� �̺�Ʈ Ÿ��
	HitSphere = 0, // Ư�� ��ġ�� �������� �浹
	SpawnActor = 1 // ��ų���� ���������� ����ϴ� ���� ����
};

USTRUCT(BlueprintType)
struct FSkillSequence
{
	GENERATED_BODY()

	FString SkillName;
	int32 ChainStep;
	EAxisInputType AxisInputType;
	EButtonInputType ButtonInputType;
	EChainType ChainType;
	class USkillObject* BindSkillObject;
};
USTRUCT(BlueprintType)
struct FSkillSequenceArray
{
	GENERATED_BODY()

	TArray<FSkillSequence> Sequences;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTB_API UCharacterSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterSkillComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void LoadSkillClass();

protected:
	UPROPERTY()
	TMap<EChainType, FSkillSequenceArray> ChainData; // <PreChainType, ChainableSkills>
	TArray<FCharacterSkillInstance> SkillClass;

protected:
	class AGameCharacter* OwnerCharacter;

protected:
	// �޺� ����
	EChainType CurChainType;
	FString NextChainSkillName; // Jump�� �޾ƿ� ��Ÿ�� ������ �̸�
	
	UPROPERTY()
	USkillObject* CurSkillObject; // ���� �������� ��ų ������Ʈ

public:
	virtual void ResetChainFlags();


public:
	// ���� ����
	void AddChainData(EChainType PreChainType, EChainType ChainType, EAxisInputType AxisType, EButtonInputType ButtonType, FString CharacterName, FString SkillName, int32 ChainStep);

public:
	// [Server + Client] ��Ÿ�� ���� �� �׿� ���ӵǴ� ������ ����
	virtual bool UseSkill(EAxisInputType AxisInput, EButtonInputType ButtonInput);
	// [Server] ��ų ������ ������ ����, ex : ���� Ÿ������, ����ü ��ȯ
	void ExecuteCustomSkillEvent(ENotifySkillType EventType);
};
