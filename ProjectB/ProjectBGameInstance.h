#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GlobalDataTable.h"
#include "ProjectBGameInstance.generated.h"

enum class EAxisInputType : uint8;
enum class EButtonInputType : uint8;
enum class EChainType : uint8;
struct FCharacterInfo;

USTRUCT(BlueprintType)
struct FCharacterSoundInfo {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* Pain;
};

USTRUCT(BlueprintType)
struct FCharacterStatInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
		float Hp;
	UPROPERTY()
		float Stemina;
};

USTRUCT(BlueprintType)
struct FEnemyInfo
{
	GENERATED_BODY()
public:
	UPROPERTY()
		FString CharacterName;
	UPROPERTY()
		float Hp;
	UPROPERTY()
		FString AnimInstRoute;
	UPROPERTY()
		USkeletalMesh* SkeletalMesh;
	UPROPERTY()
		UBehaviorTree* BTree;
	UPROPERTY()
		UBlackboardData* BBoard;
	UPROPERTY()
		FString Effect;
};

// 스킬의 속성을 관리하는 구조체
USTRUCT(BlueprintType)
struct FCharacterSkillInfo
{
	// FSkillDataTable을 스킬사용 로직에 필요한 정보만 남긴 것
	GENERATED_BODY()

	FString SkillName;

	UPROPERTY()
	UAnimMontage* AnimMontage;
	UPROPERTY()
	UParticleSystem* ImpactEffect;
	UPROPERTY()
	USoundCue* ImpactSound;

	int ComboStep;
	FName SectionName;
	TArray<float> Damage;
	
	/*-------스킬 슬롯 관련 데이터--------*/
	EChainType PreChainType; // 연계 될 체인 단계
	EChainType ChainType; // 자신의 체인 단계
	EAxisInputType AxisType; // 방향키 조작 방법
	EButtonInputType ButtonType; // 버튼 조작 방법

	/*-------Effect를 읽은 데이터--------*/
	bool bIsForceCancel; // 이전 모션을 강제로 캔슬시키는지
};
USTRUCT(BlueprintType)
struct FSkillInfoArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TArray<FCharacterSkillInfo> Infos;
};

USTRUCT(BlueprintType)
struct FCharacterSkillInstance : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY()
		FString SkillName;
	UPROPERTY()
		int32 ChainStep;

	UPROPERTY()
		class USkillObject* SkillObject;
};
USTRUCT(BlueprintType)
struct FSkillInstanceArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
		TArray<FCharacterSkillInstance> Instances;
};

UCLASS()
class PROJECTB_API UProjectBGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UProjectBGameInstance();
	virtual void Init() override;

	FCharacterStatInfo GetStatInfo(FString CharacterName);
	FCharacterSoundInfo GetSoundInfo(FString CharacterName);
	FEnemyInfo GetEnemyInfo(FString EnemyName);

	FItemDataTable GetItemInfoByIdx(int32 ItemIdx);
	FItemDataTable GetItemInfoByName(FString ItemName);

	TArray<FCharacterSkillInfo> GetSkillInfo(FString CharacterName);
	TArray<FCharacterSkillInstance> GetSkillClass(FString CharacterName);

	class USkillObject* FindSkillObject(FString CharacterName, FString SkillName, int32 ChainStep);

private:
	UPROPERTY()
	TArray<FEnemyInfo> EnemyInfo;
	UPROPERTY()
	TArray<FItemDataTable> ItemInfo;
	UPROPERTY()
	TMap<FString, FSkillInstanceArray> SkillInstanceInfo; // 스킬 오브젝트 데이터 (스킬 로직)
	UPROPERTY()
	TMap<FString, FSkillInfoArray> SkillInfo; // 스킬 데이터 (몽타주, 공격력 등)
	UPROPERTY()
	TMap<FString, FCharacterStatInfo> StatInfo; // 스탯 데이터 (초기 HP, SP 등)
	UPROPERTY()
	TMap<FString, FCharacterSoundInfo> SoundInfo; // 스탯 데이터 (초기 HP, SP 등)

	bool LoadEnemyInfo();
	bool LoadItemInfo();
	bool LoadCharacterSkill(); // 게임 내 모든 캐릭터의 스킬관련 정보 로드
	bool LoadSkillInstanceInfo(FString CharacterName, FString FileRoute); // 스킬 클래스 테이블 로드
	bool LoadSkillInfo(FString CharacterName, FString FileRoute); // 스킬 인포 테이블 로드
	bool LoadStatInfo();
	bool LoadSoundInfo();
};
