#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_InventorySlot.h"
#include <Components/WrapBox.h>
#include <Components/ScrollBox.h>
#include "UW_Inventory.generated.h"

UCLASS()
class PROJECTB_API UUW_Inventory : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
protected:
	UPROPERTY(meta = (BindWidget))
	UWrapBox* ItemSlots;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* ItemScrollBox;

	UPROPERTY()
	TSubclassOf<UUW_InventorySlot> ItemSlotWidget;
public:
	int32 GetItemSlotCount() { return ItemSlots->GetChildrenCount(); }
	void AddInventoryItemSlot(UItem* NewItem); // todo : Item - Player���� ó���ϰ� �����͸� �޾ư��� �����ʿ�
	void UpdateItemSlot(int32 ItemIdx, UItem* Item);
	void ClearItemSlots();
};
