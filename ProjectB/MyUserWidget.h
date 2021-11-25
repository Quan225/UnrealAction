#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "MyUserWidget.generated.h"

UCLASS()
class PROJECTB_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MyTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UButton* MyButton;

	void NativeConstruct() override;
	UFUNCTION()
	void ClickBindFunc();
};
