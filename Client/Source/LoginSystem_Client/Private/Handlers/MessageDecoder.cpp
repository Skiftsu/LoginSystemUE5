#include "Handlers/MessageDecoder.h"

#include "LoginSystem_Client/Protobufs/KeyMessages.pb.h"
#include "Utils/Cryptography.h"
#include "Utils/Descriptors.h"
#include "Handlers/LogicsHandler.h"

using namespace std;

MessageDecoder::MessageDecoder()
{
	logicsHandler = new LogicsHandler;
}

MessageDecoder::~MessageDecoder()
{
	delete logicsHandler;
}

void MessageDecoder::SendProtoToDecoder(const Wrapper* wrapper) const
{
	if (wrapper->has_encryptedmessage())
	{
		if(wrapper->encryptedmessage().iv().empty() || wrapper->encryptedmessage().message().empty()) return;

		const string OutMessage = Cryptography::Decrypt(wrapper->encryptedmessage().message(), wrapper->encryptedmessage().iv());
		
		switch (wrapper->encryptedmessage().messagetype())
		{
		case MT_LOGIN:
			{
				Login login;
				login.ParseFromString(OutMessage);
				logicsHandler->LoginHandler(login);
				break;
			}
		case MT_REGISTRATION:
			{
				Registration registration;
				registration.ParseFromString(OutMessage);
				logicsHandler->RegistrationHandler(registration);
				break;
			}
		default: break;
		}
	}
	else if (wrapper->has_checkregparameterstatus()) logicsHandler->CheckRegParameterHandler(wrapper->checkregparameterstatus());
	else if (wrapper->has_registration()) logicsHandler->RegistrationHandler(wrapper->registration());
	else if (wrapper->has_login()) logicsHandler->LoginHandler(wrapper->login());
	else if (wrapper->has_keyexchange()) logicsHandler->KeyExchangeHandler(wrapper->keyexchange());
}