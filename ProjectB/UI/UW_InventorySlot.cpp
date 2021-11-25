#include "UW_InventorySlot.h"
#include <ProjectB/ProjectBGameInstance.h>
#include <Math/UnrealMathUtility.h>

void UUW_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();
	ItemIcon->OnClicked.AddUniqueDynamic(this, &UUW_InventorySlot::SlotUse);
}

// DataComponent���� ������ ������ �޾Ƽ� ��� UI�� ����
void UUW_InventorySlot::UpdateSlot(UItem* UpdateItem)
{
	if (UpdateItem == nullptr)
		return;

	Item = UpdateItem;
	ItemIcon->WidgetStyle.Normal.SetResourceObject(Item->GetItemThumbnail(Item->GetItemIdx()));
}

void UUW_InventorySlot::SlotUse()
{
	// todo : UI���� �������� �� ������ ���� Idx�� �������ְ� �� ��
	// Idx -> Find Inventory -> Usable -> Use
	if (Item == nullptr)
		return;

	if (Item->GetItemIdx() == 0)
		return;

	Item->Use();
}
