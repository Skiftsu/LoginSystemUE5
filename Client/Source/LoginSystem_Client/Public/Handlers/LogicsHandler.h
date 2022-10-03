#pragma once

DECLARE_DELEGATE(FKeyExchangeDelegate);

class LogicsHandler
{
public:
	FKeyExchangeDelegate OnKeyExchange;

	void LoginHandler(class Login login);
	void RegistrationHandler(class Registration registration);
	void CheckRegParameterHandler(const class CheckRegParameterStatus RegParameterStatus);
	void KeyExchangeHandler(const class KeyExchange keyExchange) const;
};
