#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"


UCLASS()
class LOGINSYSTEM_CLIENT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
 
public:
	UPROPERTY(meta=(BindWidget))
	class UButton* wSingUpButton;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* wInfoBlock;
	
	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* wMailTextBox;
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* wPasswordTextBox;
	
	UPROPERTY(meta=(BindWidget))
	UButton* wLoginButton;
	
	UFUNCTION()
	void SingUpButtonClicked();
	UFUNCTION()
	void LoginButtonClicked();

	UFUNCTION()
	void SuccessfulAuthorization();
};
