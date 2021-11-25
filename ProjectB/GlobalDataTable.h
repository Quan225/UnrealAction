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
	// ĳ���� ��ų ������Ʈ�� �޸𸮿� �ε��ϱ� ���� ����
	// ���α׷��� �ܿ��� �ǵ帮�� �ʾƾ� ��
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
	// ĳ���� ��ų ������ ĳ���� ��ų Ŭ������ ���۷��� ��Ʈ�� ����
	// ���α׷��� �ܿ��� �ǵ帮�� �ʾƾ� ��
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
