#include "UW_Inventory.h"

// Player의 BeginPlay보다 실행순서가 늦음
void UUW_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	//FString spawnedBlueprintReference = TEXT("WidgetBlueprint'/Game/UI/WB_InventorySlot.WB_InventorySlot'");
	//UObject* loadedObject = StaticLoadObject(UObject::StaticClass(), nullptr, *spawnedBlueprintReference);
	//UBlueprint* castedBlueprint = Cast<UBlueprint>(loadedObject);

	//if (castedBlueprint)
	//{
	//	ItemSlotWidget = *castedBlueprint->GeneratedClass;

	//	if (ItemSlotWidget == nullptr)
	//	{
	//		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Cant load ItemSlot BP")));
	//		return;
	//	}
	//}
}

void UUW_Inventory::AddInventoryItemSlot(UItem* NewItem)
{
	// todo : 블루프린트 매번 로드하지 말 것
	FString spawnedBlueprintReference = TEXT("Blueprint'/Game/UI/WB_InventorySlot.WB_InventorySlot'");

	UBlueprintGeneratedClass* WB_ItemSlot = LoadObject<UBlueprintGeneratedClass>(nullptr, TEXT("Blueprint'/Game/UI/WB_InventorySlot.WB_InventorySlot_C'"));
	if (WB_ItemSlot == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("WB_ItemSlot is Nullptr"));
		return;
	}
	TSubclassOf<UUW_InventorySlot> SlotWidgetClass = Cast<UClass>(WB_ItemSlot);
	if (SlotWidgetClass == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SlotWidgetClass is Nullptr"));
		return;
	}

	UUW_InventorySlot* tempSlot = CreateWidget<UUW_InventorySlot>(GetWorld()->GetFirstPlayerController(), SlotWidgetClass);
	tempSlot->UpdateSlot(NewItem);
	ItemSlots->AddChildToWrapBox(tempSlot);

	return;

	//UObject* loadedObject = StaticLoadObject(UObject::StaticClass(), nullptr, *spawnedBlueprintReference);
	//if (loadedObject == nullptr)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("loadedObject is Nullptr"));
	//	return;
	//}

	//TSubclassOf<UUW_InventorySlot> SlotWidget = loadedObject->StaticClass();
	//if (SlotWidget == nullptr)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("SlotWidget is Nullptr"));
	//	return;
	//}

	//UBlueprint* castedBlueprint = Cast<UBlueprint>(loadedObject);
	//if (castedBlueprint == nullptr)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("castedBlueprint is Nullptr"));
	//	return;
	//}
	//
	//ItemSlotWidget = *castedBlueprint->GeneratedClass;
	//if (ItemSlotWidget == nullptr)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("ItemSlotWidget is Nullptr"));
	//	return;
	//}

	//UUW_InventorySlot* tempSlot = CreateWidget<UUW_InventorySlot>(GetWorld()->GetFirstPlayerController(), SlotWidget);
	//tempSlot->UpdateSlot(NewItem);
	//ItemSlots->AddChildToWrapBox(tempSlot);
}

void UUW_Inventory::UpdateItemSlot(int32 ItemIdx, UItem* Item)
{
	UUW_InventorySlot* UpdateSlot = Cast<UUW_InventorySlot>(ItemSlots->GetChildAt(ItemIdx));
	if (UpdateSlot == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast failed ItemSlot"));
		return;
	}

	UpdateSlot->UpdateSlot(Item);
}

void UUW_Inventory::ClearItemSlots()
{
	ItemSlots->ClearChildren();
}
