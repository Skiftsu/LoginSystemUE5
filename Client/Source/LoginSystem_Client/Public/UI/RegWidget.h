#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RegWidget.generated.h"


UCLASS()
class LOGINSYSTEM_CLIENT_API URegWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	UPROPERTY(meta=(BindWidget))
	class UEditableTextBox* wLoginTextBox;
	
	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* wPasswordTextBox;

	UPROPERTY(meta=(BindWidget))
	UEditableTextBox* wMailTextBox;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* wInfoBlock;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* wSingUpButton;
	
	UPROPERTY(meta=(BindWidget))
	UButton* wBackButton;

	UPROPERTY(meta=(BindWidget))
	class UCheckBox* wLoginCB;
	
	UPROPERTY(meta=(BindWidget))
	UCheckBox* wMailCB;

	UFUNCTION()
	void SuccessfulRegistration();
 
	bool bLoginOk = false;
	bool bMailOk = false;

private:
	UFUNCTION()
	void SingUpButtonClicked();
	UFUNCTION()
	void BackButtonClicked();
	UFUNCTION()
	void OnLoginTextChanged(const FText& text);
	UFUNCTION()
	void OnMailTextChanged(const FText& text);
};
