#include "InventoryComponent.h"
#include <ProjectB/PlayerCharacter.h>
#include <ProjectB/ProjectBGameInstance.h>

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FClassFinder<UUW_Inventory> WB_Inventory(TEXT("WidgetBlueprint'/Game/UI/WB_Inventory.WB_Inventory_C'"));
	if (WB_Inventory.Succeeded())
	{
		InventoryWidgetClass = WB_Inventory.Class;
	}	
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventoryComponent::UpdateItemSlotIcons()
{
	if (InventoryWidget == nullptr)
		return;

	for (int32 i = 0; i < HaveItems.Num(); i++)
	{
		InventoryWidget->UpdateItemSlot(i, HaveItems[i]);
	}
}

void UInventoryComponent::SetOwningCharacter(APlayerCharacter* NewOwner)
{
	OwningCharacter = NewOwner;
}

bool UInventoryComponent::CreateInventoryWidget(int32 NewCapacity)
{
	if (InventoryWidget != nullptr)
	{
		// 이미 인벤토리 UI가 생성되어 있음
		if (InventoryWidget->GetItemSlotCount() == NewCapacity)
		{
			UE_LOG(LogTemp, Log, TEXT("1"));
			return false; 
		}
		// 새로운 인벤토리가 현재 인벤토리보다 작음
		if (HaveItems.Num() > NewCapacity)
		{
			UE_LOG(LogTemp, Log, TEXT("2"));
			return false;
		}
	}

	// 컴포넌트의 소유자가 설정되지 않음
	if (OwningCharacter == nullptr)
	{

		UE_LOG(LogTemp, Log, TEXT("3"));
		return false;
	}

	if (HaveItems.Num() <= 0)
	{
		for (int32 i = 0; i < NewCapacity; i++)
		{
			UItem* tempItem = NewObject<UItem>(this, UItem::StaticClass());
			tempItem->SetItemIdx(0);
			tempItem->SetOwingInventory(this);
			tempItem->SetIsStackable(false);

			HaveItems.Add(tempItem);
		}
	}

	// Inventory UI 생성
	if (!IsRunningDedicatedServer())
	{
		// 위젯을 출력할 컨트롤러가 지정되지 않음
		if (OwningCharacter->GetController() == nullptr)
		{

			UE_LOG(LogTemp, Log, TEXT("4"));
			return false;
		}
		if (InventoryWidgetClass == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Class Null"));
			return false;
		}

		InventoryWidget = CreateWidget<UUW_Inventory>(Cast<APlayerController>(OwningCharacter->GetController()), InventoryWidgetClass);
		if (InventoryWidget == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Widget Null"));
			return false;
		}

		InventoryWidget->ClearItemSlots();
		for (int32 i = 0; i < HaveItems.Num(); i++)
		{
			InventoryWidget->AddInventoryItemSlot(HaveItems[i]);
		}
	}

	


	
	return true;
}

void UInventoryComponent::Server_UseItem_Implementation(int32 UseItemIdx)
{
	if (OwningCharacter == nullptr)
		return;

	if (OwningCharacter->GetLocalRole() < ROLE_Authority)
		return;

	UItem* UseItem = FindItem(UseItemIdx);
	if (UseItem == nullptr)
		return;

	UseItem->ApplyItemEffect(UseItem->GetItemEffect(UseItem->GetItemIdx()));
	
	int32 ReduceItemCount = UseItem->GetItemCount() - 1;
	if (ReduceItemCount <= 0)
	{
		UseItem->Remove();
	}
	else
	{
		UseItem->SetItemCount(ReduceItemCount);
	}
	
	Client_UseItem(UseItemIdx);
}

void UInventoryComponent::Client_UseItem_Implementation(int32 UseItemIdx)
{
	if (OwningCharacter == nullptr)
		return;

	if (OwningCharacter->GetLocalRole() == ROLE_Authority)
		return;

	UItem* UseItem = FindItem(UseItemIdx);
	if (UseItem == nullptr)
		return;

	UseItem->ApplyItemEffect(UseItem->GetItemEffect(UseItem->GetItemIdx()));

	int32 ReduceItemCount = UseItem->GetItemCount() - 1;
	if (ReduceItemCount <= 0)
	{
		UseItem->Remove();
	}
	else
	{
		UseItem->SetItemCount(ReduceItemCount);
	}

	UpdateItemSlotIcons();
}

UItem* UInventoryComponent::FindItem(int32 UseItemIdx)
{
	for (int32 i = 0; i < HaveItems.Num(); i++)
	{
		if (HaveItems[i]->GetItemIdx() == UseItemIdx && HaveItems[i]->GetItemCount() > 0)
			return HaveItems[i];
	}

	return nullptr;
}

bool UInventoryComponent::AddItem(UItem* Item, int32 AddCount)
{
	if (Item == nullptr)
		return false;


	if (Item->GetIsStackable())
	{
		// 동일 아이템이 존재하는지 IDX를 기준으로 탐색
		for (int32 i = 0; i < HaveItems.Num(); i++)
		{
			if (HaveItems[i]->GetItemCount() == 0)
				continue;

			if (HaveItems[i]->GetItemIdx() != Item->GetItemIdx())
				continue;

			HaveItems[i]->AddItemCount(AddCount);
			UpdateItemSlotIcons();
			return true;
		}
	}
	else
	{
		for (int32 i = 0; i < HaveItems.Num(); i++)
		{
			if (HaveItems[i]->GetItemCount() == 0 || HaveItems[i]->GetItemIdx() == 0)
			{
				Item->AddItemCount(AddCount);
				Item->SetOwingInventory(this);
				HaveItems[i] = Item;
				UpdateItemSlotIcons();
				return true;
			}
		}
	}

	return false;
}

bool UInventoryComponent::AddItem(int32 NewItemIdx, int32 NewItemCount)
{
	UProjectBGameInstance* Instance = GetWorld()->GetGameInstance<UProjectBGameInstance>();
	if (Instance == nullptr)
		return false;

	FItemDataTable ItemInfo = Instance->GetItemInfoByIdx(NewItemIdx);
	if (ItemInfo.ItemIdx == 0)
		return false;

	UItem* NewItem = NewObject<UItem>();
	NewItem->SetItemIdx(ItemInfo.ItemIdx);
	NewItem->SetItemName(ItemInfo.ItemName);
	NewItem->SetIsStackable(ItemInfo.bIsStackable);

	return AddItem(NewItem, NewItemCount);
}

bool UInventoryComponent::CanAddItem()
{
	return true;
}

//bool UInventoryComponent::ReduceItem(UItem* Item, int32 ReduceCount)
//{
//	if (Item == nullptr)
//		return false;
//
//	if (Item->GetItemCount() < ReduceCount)
//		return false;
//
//	int32 RemoveIdx = HaveItems.Find(Item);
//	if (RemoveIdx == INDEX_NONE || RemoveIdx >= HaveItems.Num())
//		return false;
//
//	//InventoryWidget->UpdateItemSlot(RemoveIdx, HaveItems[RemoveIdx]);
//	//UpdateItemSlotIcons();
//
//	return true;
//}

//bool UInventoryComponent::RemoveItem(UItem* Item)
//{
//	if (Item == nullptr)
//		return false;
//
//	int32 RemoveIdx = HaveItems.Find(Item);
//	if (RemoveIdx == INDEX_NONE || RemoveIdx >= HaveItems.Num())
//		return false;
//
//	HaveItems[RemoveIdx]->SetItemIdx(0);
//	HaveItems[RemoveIdx]->AddItemCount(HaveItems[RemoveIdx]->GetItemCount());
//
//	return true;
//}

void UInventoryComponent::OpenInventory()
{
	if(InventoryWidget != nullptr)
		InventoryWidget->AddToViewport();
}

void UInventoryComponent::CloseInventory()
{
	if (InventoryWidget != nullptr)
		InventoryWidget->RemoveFromViewport();
}

APlayerCharacter* UInventoryComponent::GetOwningCharacter()
{
	return OwningCharacter;
}

