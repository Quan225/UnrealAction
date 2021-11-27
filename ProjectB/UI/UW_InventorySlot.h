#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/Image.h>
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
	UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton;
	
	UPROPERTY()
	UItem* Item;

public:
	void UpdateSlot(UItem* UpdateItem = nullptr);

	UFUNCTION()
	void SlotUse();
};
