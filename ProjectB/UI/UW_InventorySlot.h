#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <ProjectB/Inventory/Item.h>
#include "UW_InventorySlot.generated.h"

UCLASS()
class PROJECTB_API UUW_InventorySlot : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* ItemIcon;
	
	UPROPERTY()
	UItem* Item;

public:
	UButton* GetIcon() { return ItemIcon; }

	void UpdateSlot(UItem* UpdateItem = nullptr);

	UFUNCTION()
	void SlotUse();
};
