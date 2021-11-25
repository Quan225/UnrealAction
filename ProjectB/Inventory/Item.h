#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Texture2D.h"
#include "Item.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	None = 0,
	Equip = 1,
	Useable = 2
};

UCLASS()
class PROJECTB_API UItem : public UObject
{
	GENERATED_BODY()
public:
	UItem();

protected:
	// todo : �������� ���븸 ����� UI���� DataTable�� �б� ������ �ʿ���°� �����Ұ�
	UPROPERTY()
	FString ItemName;

	UPROPERTY()
	FString Description;

	UPROPERTY()
	EItemType ItemType;

	UPROPERTY()
	bool bIsStackable;

	UPROPERTY()
	int32 ItemCount;

	UPROPERTY()
	int32 ItemIdx;

	UPROPERTY()
	class UInventoryComponent* OwningInventory;

	//class UProjectBGameInstance* GameInstance; // �����ڿ��� �ʱ�ȭ���ڴ� World�� ��� �׾������... ��¼��...

public:
	int32 GetItemIdx() { return ItemIdx; }
	int32 GetItemCount() { return ItemCount; }
	bool GetIsStackable() { return bIsStackable; }

	void AddItemCount(int32 AddCount) { ItemCount += AddCount; }
	void SetItemIdx(int32 NewItemIdx) { ItemIdx = NewItemIdx; }
	void SetIsStackable(bool NewIsStackable) { bIsStackable = NewIsStackable; }
	void SetDescription(FString NewDescription) { Description = NewDescription; }
	void SetItemName(FString NewItemName) { ItemName = NewItemName; }
	void SetItemCount(int32 NewItemCount) { ItemCount = NewItemCount; }

public:
	UTexture2D* GetItemThumbnail(int32 ReadIdx);
	FString GetItemName(int32 ReadIdx);
	USkeletalMesh* GetItemMesh(int32 ReadIdx);
	FString GetItemEffect(int32 ReadIdx);


public:
	void Use();
	void Remove();
	virtual void ApplyItemEffect(FString Effect);

	void SetOwingInventory(class UInventoryComponent* Inventory);
};
