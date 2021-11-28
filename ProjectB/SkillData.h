#pragma once
// ĳ���� ���� ������ ���̺� ����

#include <Engine/DataTable.h>
#include <Engine/Classes/Animation/AnimMontage.h>
#include <Sound/SoundCue.h>
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterSkillComponent.h"
#include "SkillData.generated.h"


USTRUCT(BlueprintType)
struct FCharacterInfo : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CharacterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Stemina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString AnimInstRoute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBehaviorTree* BTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBlackboardData* BBoard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Effect; // ��Ÿ �ɼ�
};

// ĳ������ ��ų ���� ���̺�
USTRUCT(BlueprintType)
struct FSkillDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SkillName; // ��ų��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AnimMontage; // ��Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* ImpactEffect; // Ÿ�ݽ� ����� ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* ImpactSound; // Ÿ�ݽ� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ComboStep; // ü�ΰ����� ��� �ܰ�. ü���� �ƴ� �� 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName SectionName; // �ش� ��ų�� �����ϱ� ���� ������ �̸�. ������ ���� �� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RequireStemina; // ��ų ��뿡 �ʿ��� ���׹̳�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> Damage; // ������. �ٴ���Ʈ �� ��� ���� �������� ����� Index�� �������� �ۼ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EChainType PreChainType; // ���� �� ü�� �ܰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EChainType ChainType; // �ڽ��� ü�� �ܰ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAxisInputType AxisType; // ����Ű ���� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EButtonInputType ButtonType; // ��ư ���� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Effect; // ��Ÿ �ɼ�
};

USTRUCT(BlueprintType)
struct FCharacterStatDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CharacterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Stemina;
};

USTRUCT(BlueprintType)
struct FCharacterSound : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CharacterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* Pain;
};

// �����ͻ� �߰� �ϱ����� ���� ����
UCLASS()
class PROJECTB_API ACharacterDataTables : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterDataTables();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
