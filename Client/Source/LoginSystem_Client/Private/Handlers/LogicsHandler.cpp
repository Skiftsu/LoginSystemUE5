#include "Handlers/LogicsHandler.h"

#include "LoginSystem_Client/Protobufs/KeyMessages.pb.h"
#include "Handlers/MessageEncoder.h"
#include "UI/LoginWidget.h"
#include "UI/RegWidget.h"
#include "UI/MainMenuHUD.h"
#include "Utils/Cryptography.h"
#include "Utils/Descriptors.h"

void LogicsHandler::LoginHandler(Login login)
{
	if(login.state() == MS_SUCCESSFUL) AMainMenuHUD::LoginWidget->SuccessfulAuthorization();
}

void LogicsHandler::RegistrationHandler(Registration registration)
{
	if(registration.state() == MS_SUCCESSFUL) AMainMenuHUD::RegistrationWidget->SuccessfulRegistration();
}

void LogicsHandler::CheckRegParameterHandler(const CheckRegParameterStatus RegParameterStatus)
{
	if (RegParameterStatus.GetReflection()->HasField(RegParameterStatus, Descriptors::loginStatus_ich))
		AMainMenuHUD::RegistrationWidget->bLoginOk = RegParameterStatus.loginstatus();
	else if (RegParameterStatus.GetReflection()->HasField(RegParameterStatus, Descriptors::mailStatus_ich))
		AMainMenuHUD::RegistrationWidget->bMailOk = RegParameterStatus.mailstatus();
}

void LogicsHandler::KeyExchangeHandler(const KeyExchange keyExchange) const
{
	if(keyExchange.publickey().empty() || keyExchange.state() == MS_ERROR) return;
	
	if (keyExchange.state() == MS_INIT)
	{
		GLog->Log("Accept public key");
				
		KeyExchange* Message = new KeyExchange;
		Message->set_publickey(Cryptography::GenerateKeyPair_DH());
		Message->set_state(MS_SUCCESSFUL);
		UMessageEncoder::Send(Message);
	}

	Cryptography::ComputeSharedSecretKey_DH(keyExchange.publickey());
	OnKeyExchange.Execute();
}