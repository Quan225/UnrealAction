#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <ProjectB/ProjectBGameInstance.h>
#include "CharacterSkillComponent.generated.h"

UENUM(BlueprintType)
enum class EAxisInputType : uint8
{
	// 스킬을 발동시키기 위해 요구되는 방향 추가입력
	None = 0,
	Front = 1,
	Back = 2,
	Right = 3,
	Left = 4
};

UENUM(BlueprintType)
enum class EButtonInputType : uint8
{
	// 스킬을 발동시키기 위해 요구되는 버튼 추가입력
	None = 0,
	Skill_Q = 1,
	Space = 2,
};

UENUM(BlueprintType)
enum class EChainType : uint8
{
	// 스킬을 발동시킬 수 있는 단계
	None = 0, // 상시 사용가능
	Basic_1 = 1, // Basic_1 타입의 스킬 발동 중 파생 가능
	Basic_2 = 2,
	Basic_3 = 3,
	Basic_4 = 4,
	Skill_Q = 5,
	Skill_R = 6
};

UENUM(BlueprintType)
enum class ENotifySkillType : uint8
{
	// 스킬 노티파이에서 발생시킬 스킬의 이벤트 타입
	HitSphere = 0, // 특정 위치에 원형으로 충돌
	SpawnActor = 1 // 스킬과는 독립적으로 기능하는 액터 생성
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
	// 콤보 관련
	EChainType CurChainType;
	FString NextChainSkillName; // Jump시 받아올 몽타주 섹션의 이름
	
	UPROPERTY()
	USkillObject* CurSkillObject; // 현재 실행중인 스킬 오브젝트

public:
	virtual void ResetChainFlags();


public:
	// 세팅 관련
	void AddChainData(EChainType PreChainType, EChainType ChainType, EAxisInputType AxisType, EButtonInputType ButtonType, FString CharacterName, FString SkillName, int32 ChainStep);

public:
	// [Server + Client] 몽타주 실행 및 그에 종속되는 동작을 정의
	virtual bool UseSkill(EAxisInputType AxisInput, EButtonInputType ButtonInput);
	// [Server] 스킬 고유의 로직을 실행, ex : 원형 타격판정, 투사체 소환
	void ExecuteCustomSkillEvent(ENotifySkillType EventType);
};
