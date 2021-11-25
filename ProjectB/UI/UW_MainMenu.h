#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MainMenu.generated.h"

UCLASS()
class PROJECTB_API UUW_MainMenu : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerAddressTextBox;;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	void NativeConstruct() override;

	UFUNCTION()
	void JoinServer();
	UFUNCTION()
	void QuitGame();

};
