#include "UI/LoginWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "LoginSystem_Client/Protobufs/KeyMessages.pb.h"
#include "Handlers/MessageEncoder.h"
#include "Networking/SocketObject.h"
#include "UI/MainMenuHUD.h"
#include "Utils/Cryptography.h"


void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	wSingUpButton->OnClicked.AddDynamic(this, &ULoginWidget::SingUpButtonClicked);
	wLoginButton->OnClicked.AddDynamic(this, &ULoginWidget::LoginButtonClicked);
}

void ULoginWidget::SingUpButtonClicked()
{
	if (USocketObject::bIsConnection) Cast<AMainMenuHUD>(GetOwningPlayer()->GetHUD())->RegistrationWidgetVisibility(true);
	else GLog->Log("No connection");
}

void ULoginWidget::LoginButtonClicked()
{
	if (USocketObject::bIsConnection)
	{
		Login* Message = new Login;
		Message->set_mail(TCHAR_TO_UTF8(*wMailTextBox->GetText().ToString()));
		Message->set_hash(Cryptography::SHA256(*wPasswordTextBox->GetText().ToString()));
		Message->set_state(MS_INIT);
		
		UMessageEncoder::Send(Message, true);
	}
	else GLog->Log("No connection");
}

void ULoginWidget::SuccessfulAuthorization()
{
	Cast<AMainMenuHUD>(GetOwningPlayer()->GetHUD())->MainMenuWidgetVisibility(true);
}
