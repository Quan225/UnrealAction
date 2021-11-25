#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "UseableItem.generated.h"

// Not Use
// Item이 상속관계를 가질 정도로 분리될 기능을 가지고 있지 않음
UCLASS()
class PROJECTB_API UUseableItem : public UItem
{
	GENERATED_BODY()
public:
	UUseableItem();
};
