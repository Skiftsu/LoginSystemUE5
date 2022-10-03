#include "UI/RegWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "LoginSystem_Client/Protobufs/KeyMessages.pb.h"
#include "Handlers/MessageEncoder.h"
#include "Components/CheckBox.h"
#include "UI/MainMenuHUD.h"
#include "Utils/Cryptography.h"


void URegWidget::NativeConstruct()
{
	Super::NativeConstruct();

	wLoginTextBox->OnTextChanged.AddDynamic(this, &URegWidget::OnLoginTextChanged);
	wMailTextBox->OnTextChanged.AddDynamic(this, &URegWidget::OnMailTextChanged);
	wSingUpButton->OnClicked.AddDynamic(this, &URegWidget::SingUpButtonClicked);
	wBackButton->OnClicked.AddDynamic(this, &URegWidget::BackButtonClicked);
}

void URegWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	wLoginCB->SetCheckedState(bLoginOk ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	wMailCB->SetCheckedState(bMailOk ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void URegWidget::SuccessfulRegistration(){BackButtonClicked();}

void URegWidget::SingUpButtonClicked()
{
	if (bLoginOk && bMailOk)
	{
		Registration* Message = new Registration;
		Message->set_login(TCHAR_TO_UTF8(*wLoginTextBox->GetText().ToString()));
		Message->set_mail(TCHAR_TO_UTF8(*wMailTextBox->GetText().ToString()));
		Message->set_hash(Cryptography::SHA256(wPasswordTextBox->GetText().ToString()));
		Message->set_state(MS_INIT);

		UMessageEncoder::Send(Message, true);
	}
	else wInfoBlock->SetText(FText::FromString("Error : Enter valid login/mail"));
}

void URegWidget::BackButtonClicked(){Cast<AMainMenuHUD>(GetOwningPlayer()->GetHUD())->LoginWidgetVisibility(true);}

void URegWidget::OnLoginTextChanged(const FText& text)
{
	if(text.IsEmpty()) return;
	CheckRegParameterStatus* Message = new CheckRegParameterStatus;
	Message->set_login(TCHAR_TO_UTF8(*text.ToString()));
	UMessageEncoder::Send(Message);
}

void URegWidget::OnMailTextChanged(const FText& text)
{
	if(text.IsEmpty()) return;
	CheckRegParameterStatus* Message = new CheckRegParameterStatus;
	Message->set_mail(TCHAR_TO_UTF8(*text.ToString()));
	UMessageEncoder::Send(Message);
}
