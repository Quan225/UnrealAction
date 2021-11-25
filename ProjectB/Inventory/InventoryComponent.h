#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item.h"
#include <ProjectB/UI/UW_Inventory.h>
#include "InventoryComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTB_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY()
	TSubclassOf<UUW_Inventory> InventoryWidgetClass;
	
	UPROPERTY()
	UUW_Inventory* InventoryWidget; // 이거 맞는거?

protected:
	UPROPERTY()
	TArray<UItem*> HaveItems;
	UPROPERTY()
	class APlayerCharacter* OwningCharacter;

public:
	bool CreateInventoryWidget(int32 NewCapacity);

	UFUNCTION(Server, Reliable)
	void Server_UseItem(int32 UseItemIdx);
	UFUNCTION(Client, Reliable)
	void Client_UseItem(int32 UseItemIdx);
	UFUNCTION()
	UItem* FindItem(int32 UseItemIdx);

	bool AddItem(UItem* Item, int32 AddCount);
	bool AddItem(int32 NewItemIdx, int32 NewItemCount);
	bool CanAddItem();

// 만들긴 했는데 인벤토리에서 아이템의 속성을 건드리는건 없어야 해서 주석처리
//	bool ReduceItem(UItem* Item, int32 ReduceCount);
//	bool RemoveItem(UItem* Item);
	void UpdateItemSlotIcons();

	void OpenInventory();
	void CloseInventory();

	void SetOwningCharacter(class APlayerCharacter* NewOwner);
	class APlayerCharacter* GetOwningCharacter();
};
