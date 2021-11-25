#include "UW_MainMenu.h"
#include <Components/EditableTextBox.h>
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>

void UUW_MainMenu::NativeConstruct()
{
	JoinButton->OnClicked.AddUniqueDynamic(this, &UUW_MainMenu::JoinServer);
}

void UUW_MainMenu::JoinServer()
{
	UE_LOG(LogTemp, Log, TEXT("Try Join : %s"), *ServerAddressTextBox->Text.ToString());

	UGameplayStatics::OpenLevel(GetWorld(), FName(*ServerAddressTextBox->Text.ToString()));
}

void UUW_MainMenu::QuitGame()
{
}
