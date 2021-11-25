#include "MyUserWidget.h"

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MyButton->OnClicked.AddUniqueDynamic(this, &UMyUserWidget::ClickBindFunc);
}

void UMyUserWidget::ClickBindFunc()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Click"));
}
