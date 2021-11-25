#pragma once

#include "CoreMinimal.h"
#include <Engine/DataTable.h>
#include "Engine/Texture2D.h"
#include "Inventory/Item.h"
#include "GameFramework/Actor.h"
#include "GlobalDataTable.generated.h"

USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ItemIdx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* Thumbnail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* InWorldStaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsStackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ItemEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Description;
};

USTRUCT(BlueprintType)
struct FCharacterSkillClass : public FTableRowBase
{
	// 캐릭터 스킬 오브젝트를 메모리에 로드하기 위한 정보
	// 프로그래머 외에는 건드리지 않아야 함
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SkillName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 ChainStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString ReferenceRoute;
};

USTRUCT(BlueprintType)
struct FCharacterSkill : public FTableRowBase
{
	// 캐릭터 스킬 인포와 캐릭터 스킬 클래스의 레퍼런스 루트를 저장
	// 프로그래머 외에는 건드리지 않아야 함
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString CharacterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SkillClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString SkillInfo;
};

UCLASS()
class PROJECTB_API AGlobalDatas : public AActor
{
	GENERATED_BODY()
	
public:	
	AGlobalDatas();
};
