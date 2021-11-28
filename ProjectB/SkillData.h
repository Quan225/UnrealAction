#pragma once
// 캐릭터 관련 데이터 테이블 묶음

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
		FString Effect; // 기타 옵션
};

// 캐릭터의 스킬 정보 테이블
USTRUCT(BlueprintType)
struct FSkillDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SkillName; // 스킬명
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* AnimMontage; // 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem* ImpactEffect; // 타격시 출력할 이펙트
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundCue* ImpactSound; // 타격시 실행할 사운드
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ComboStep; // 체인공격일 경우 단계. 체인이 아닐 시 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName SectionName; // 해당 스킬을 실행하기 위한 섹션의 이름. 섹션이 없을 시 공백
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RequireStemina; // 스킬 사용에 필요한 스테미나
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<float> Damage; // 데미지. 다단히트 할 경우 세부 로직에서 사용할 Index를 기준으로 작성
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EChainType PreChainType; // 연계 될 체인 단계
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EChainType ChainType; // 자신의 체인 단계
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EAxisInputType AxisType; // 방향키 조작 방법
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EButtonInputType ButtonType; // 버튼 조작 방법
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Effect; // 기타 옵션
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

// 에디터상에 뜨게 하기위한 더미 액터
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
