#include "UW_InventorySlot.h"
#include <ProjectB/ProjectBGameInstance.h>
#include <Math/UnrealMathUtility.h>

void UUW_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	ItemIcon->OnClicked.AddUniqueDynamic(this, &UUW_InventorySlot::SlotUse);
}

// DataComponent에서 아이템 정보를 받아서 출력 UI를 갱신
void UUW_InventorySlot::UpdateSlot(UItem* UpdateItem)
{
	if (UpdateItem == nullptr)
		return;

	Item = UpdateItem;
	ItemIcon->WidgetStyle.Normal.SetResourceObject(Item->GetItemThumbnail(Item->GetItemIdx()));
}

void UUW_InventorySlot::SlotUse()
{
	// todo : UI에서 아이템을 다 가지지 말고 Idx만 가지고있게 할 것
	// Idx -> Find Inventory -> Usable -> Use
	if (Item == nullptr)
		return;

	if (Item->GetItemIdx() == 0)
		return;

	Item->Use();
}
