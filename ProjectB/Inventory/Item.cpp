#include "Item.h"
#include "InventoryComponent.h"
#include <ProjectB/PlayerCharacter.h>
#include <ProjectB/ProjectBGameInstance.h>

UItem::UItem()
{
}

UTexture2D* UItem::GetItemThumbnail(int32 ReadIdx)
{
	UWorld* World = GetWorld();
	if (World == nullptr && OwningInventory != nullptr)
	{
		World = OwningInventory->GetWorld();
		if (World == nullptr)
			return nullptr;
	}

	UProjectBGameInstance* GameInstance = Cast<UProjectBGameInstance>(World->GetGameInstance());
	return GameInstance->GetItemInfoByIdx(ReadIdx).Thumbnail;
}

FString UItem::GetItemName(int32 ReadIdx)
{
	UWorld* World = GetWorld();
	if (World == nullptr && OwningInventory != nullptr)
	{
		World = OwningInventory->GetWorld();
		if (World == nullptr)
			return FString();
	}

	UProjectBGameInstance* GameInstance = Cast<UProjectBGameInstance>(World->GetGameInstance());
	return GameInstance->GetItemInfoByIdx(ReadIdx).ItemName;
}

USkeletalMesh* UItem::GetItemMesh(int32 ReadIdx)
{
	UWorld* World = GetWorld();
	if (World == nullptr && OwningInventory != nullptr)
	{
		World = OwningInventory->GetWorld();
		if (World == nullptr)
			return nullptr;
	}

	UProjectBGameInstance* GameInstance = Cast<UProjectBGameInstance>(World->GetGameInstance());
	return GameInstance->GetItemInfoByIdx(ReadIdx).InWorldStaticMesh;
}

FString UItem::GetItemEffect(int32 ReadIdx)
{
	UWorld* World = GetWorld();
	if (World == nullptr && OwningInventory != nullptr)
	{
		World = OwningInventory->GetWorld();
		if (World == nullptr)
			return FString();
	}

	UProjectBGameInstance* GameInstance = Cast<UProjectBGameInstance>(World->GetGameInstance());
	return GameInstance->GetItemInfoByIdx(ReadIdx).ItemEffect;
}

void UItem::Use()
{
	if (OwningInventory == nullptr)
		return;

	if (ItemCount <= 0)
		return;
	
	OwningInventory->Server_UseItem(ItemIdx);
}

void UItem::Remove()
{
	ItemIdx = 0;
	ItemCount = 0;
}

void UItem::ApplyItemEffect(FString Effect)
{
	if (OwningInventory == nullptr)
		return;

	TArray<FString> EffectArr = {};
	Effect.ParseIntoArray(EffectArr, TEXT(" "));

	for (int32 i = 0; i < EffectArr.Num(); i++)
	{
		FString CurEffect = *EffectArr[i];
		if(CurEffect.Compare(TEXT("HealHp")) == 0)
		{ 
			float CurHp = OwningInventory->GetOwningCharacter()->GetCurHp();
			OwningInventory->GetOwningCharacter()->SetCurHp(CurHp + 30.0f);
		}
		if (CurEffect.Compare(TEXT("HealStamina")) == 0)
		{
			float CurStamina = OwningInventory->GetOwningCharacter()->GetCurStamina();
			OwningInventory->GetOwningCharacter()->SetCurStamina(CurStamina + 30.0f);
		}
	}
}

void UItem::SetOwingInventory(UInventoryComponent* Inventory)
{
	OwningInventory = Inventory;
}
