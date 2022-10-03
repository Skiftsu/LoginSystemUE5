#include "UI/MainMenuWidget.h"

#include "Components/Button.h"
#include "UI/MainMenuHUD.h"
#include "UI/LoginWidget.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	wLogOutButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LogOutBtnClicked);
}

void UMainMenuWidget::LogOutBtnClicked()
{
	Cast<AMainMenuHUD>(GetOwningPlayer()->GetHUD())->LoginWidgetVisibility(true);
}