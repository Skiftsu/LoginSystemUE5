#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class ULoginWidget;
class URegWidget;
class UMainMenuWidget;

UCLASS()
class LOGINSYSTEM_CLIENT_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	void RegistrationWidgetVisibility(bool Visibility);
	void LoginWidgetVisibility(bool Visibility);
	void MainMenuWidgetVisibility(bool Visibility);
	virtual void BeginDestroy() override;

	static inline ULoginWidget* LoginWidget;
	static inline URegWidget* RegistrationWidget;
	static inline UMainMenuWidget* MainMenuWidget;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<URegWidget> RegWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<ULoginWidget> LoginWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	TArray<UUserWidget*> CreatedWidgets;

	UFUNCTION()
	void HideAllWidgets();
};