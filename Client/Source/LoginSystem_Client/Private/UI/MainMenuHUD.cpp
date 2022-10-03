#include "UI/MainMenuHUD.h"

#include "UI/LoginWidget.h"
#include "UI/MainMenuWidget.h"
#include "UI/RegWidget.h"


void AMainMenuHUD::RegistrationWidgetVisibility(bool Visibility)
{
	if(!(RegistrationWidget && IsValid(RegistrationWidget)))
	{
		if (Visibility == false) return;
		
		RegistrationWidget = CreateWidget<URegWidget>(GetWorld(), RegWidgetClass);
		CreatedWidgets.Add(RegistrationWidget);
		RegistrationWidget->AddToViewport();
	}

	if(Visibility)
	{
		HideAllWidgets();
		RegistrationWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else RegistrationWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMainMenuHUD::LoginWidgetVisibility(bool Visibility)
{
	if(!(LoginWidget && IsValid(LoginWidget)))
	{
		if (Visibility == false) return;
		
		LoginWidget = CreateWidget<ULoginWidget>(GetWorld(), LoginWidgetClass);
		CreatedWidgets.Add(LoginWidget);
		LoginWidget->AddToViewport();
	}

	if(Visibility)
	{
		HideAllWidgets();
		LoginWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else LoginWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMainMenuHUD::MainMenuWidgetVisibility(bool Visibility)
{
	if(!(MainMenuWidget && IsValid(MainMenuWidget)))
	{
		if (Visibility == false) return;
		
		MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		CreatedWidgets.Add(MainMenuWidget);
		MainMenuWidget->AddToViewport();
	}

	if(Visibility)
	{
		HideAllWidgets();
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else MainMenuWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMainMenuHUD::BeginDestroy()
{
	Super::BeginDestroy();

	LoginWidget = nullptr;
	RegistrationWidget = nullptr;
	MainMenuWidget = nullptr;
}

void AMainMenuHUD::HideAllWidgets()
{
	for(const auto Widget : CreatedWidgets) Widget->SetVisibility(ESlateVisibility::Hidden);
}

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	MyController->bShowMouseCursor = true;

	LoginWidgetVisibility(true);
}
