#pragma once

class Wrapper;
class LogicsHandler;

class MessageDecoder
{
public:
	MessageDecoder();
	~MessageDecoder();

	void SendProtoToDecoder(const Wrapper* wrapper) const;
	LogicsHandler* logicsHandler;
};
