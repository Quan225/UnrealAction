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

// ��ų�� �Ӽ��� �����ϴ� ����ü
USTRUCT(BlueprintType)
struct FCharacterSkillInfo
{
	// FSkillDataTable�� ��ų��� ������ �ʿ��� ������ ���� ��
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
	
	/*-------��ų ���� ���� ������--------*/
	EChainType PreChainType; // ���� �� ü�� �ܰ�
	EChainType ChainType; // �ڽ��� ü�� �ܰ�
	EAxisInputType AxisType; // ����Ű ���� ���
	EButtonInputType ButtonType; // ��ư ���� ���

	/*-------Effect�� ���� ������--------*/
	bool bIsForceCancel; // ���� ����� ������ ĵ����Ű����
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
	TMap<FString, FSkillInstanceArray> SkillInstanceInfo; // ��ų ������Ʈ ������ (��ų ����)
	UPROPERTY()
	TMap<FString, FSkillInfoArray> SkillInfo; // ��ų ������ (��Ÿ��, ���ݷ� ��)
	UPROPERTY()
	TMap<FString, FCharacterStatInfo> StatInfo; // ���� ������ (�ʱ� HP, SP ��)
	UPROPERTY()
	TMap<FString, FCharacterSoundInfo> SoundInfo; // ���� ������ (�ʱ� HP, SP ��)

	bool LoadEnemyInfo();
	bool LoadItemInfo();
	bool LoadCharacterSkill(); // ���� �� ��� ĳ������ ��ų���� ���� �ε�
	bool LoadSkillInstanceInfo(FString CharacterName, FString FileRoute); // ��ų Ŭ���� ���̺� �ε�
	bool LoadSkillInfo(FString CharacterName, FString FileRoute); // ��ų ���� ���̺� �ε�
	bool LoadStatInfo();
	bool LoadSoundInfo();
};
