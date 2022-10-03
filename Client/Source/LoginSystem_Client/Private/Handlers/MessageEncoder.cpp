#include "Handlers/MessageEncoder.h"
#include "LoginSystem_Client/Protobufs/KeyMessages.pb.h"
#include "Handlers/LogicsHandler.h"
#include "Handlers/MessageDecoder.h"
#include "Networking/SocketListener.h"
#include "Networking/SocketObject.h"
#include "Utils/Cryptography.h"

using namespace std;

bool UMessageEncoder::Send(google::protobuf::Message* Message, const bool Encrypt)
{
	if(!Message) return false;
	if(Encrypt)
	{
		KeyExchange* keyExchange = new KeyExchange;
		keyExchange->set_publickey(Cryptography::GenerateKeyPair_DH());
		keyExchange->set_state(MS_INIT);
		
		static google::protobuf::Message* SavedMessage;
		if(SavedMessage) delete SavedMessage;
		SavedMessage = Message;
		
		FSocketListener::MessageDecoderObj->logicsHandler->OnKeyExchange.BindLambda([&]{SendProtobufMessage(WrapMessage(SavedMessage, true)); SavedMessage = nullptr;});

		return SendProtobufMessage(WrapMessage(keyExchange, false));
	}
	return SendProtobufMessage(WrapMessage(Message, Encrypt));
}

bool UMessageEncoder::SendAliveMessage()
{
	const Wrapper AliveMessage;
	return SendProtobufMessage(AliveMessage);
}

bool UMessageEncoder::SendProtobufMessage(const Wrapper Message)
{
	TArray<uint8> Buffer;
	Buffer.AddUninitialized(Message.ByteSizeLong()+5);
	
	google::protobuf::io::ArrayOutputStream ArrayOutput(Buffer.GetData(), Buffer.Num());
	google::protobuf::io::CodedOutputStream CodedOutput(&ArrayOutput);
	CodedOutput.WriteVarint32(Message.ByteSizeLong());
	Message.SerializeToCodedStream(&CodedOutput);

	int32 BytesSent;
	return USocketObject::Socket->Send(Buffer.GetData(), CodedOutput.ByteCount(), BytesSent);
}

bool UMessageEncoder::EncryptProtobufMessage(const google::protobuf::Message* Message, EncryptedMessage* EncryptMessage)
{
	TArray<uint8> ProtoArr;
	ProtoArr.AddUninitialized(Message->ByteSizeLong());
	Message->SerializeToArray(ProtoArr.GetData(), ProtoArr.Num());

	string OutMessage, OutIv;
	Cryptography::Encrypt(ProtoArr, OutMessage, OutIv);
	EncryptMessage->set_message(OutMessage);
	EncryptMessage->set_iv(OutIv);
	return true;
}

Wrapper UMessageEncoder::WrapMessage(google::protobuf::Message* Message, const bool Encrypt)
{
	Wrapper wrapper;
	if(!Message) return wrapper;

	if (Encrypt)
	{
		const TUniquePtr<google::protobuf::Message> MessageUnqPtr(Message);
		TUniquePtr<EncryptedMessage> EncryptMessage(new EncryptedMessage);
		EncryptProtobufMessage(MessageUnqPtr.Get(), EncryptMessage.Get());
		
		if (Message->GetTypeName() == "Registration") EncryptMessage->set_messagetype(MT_REGISTRATION);
		else if (Message->GetTypeName() == "Login") EncryptMessage->set_messagetype(MT_LOGIN);
		else return wrapper;
		
		const auto EncryptMsgPtr = EncryptMessage.Get();
		EncryptMessage.Release();
		wrapper.set_allocated_encryptedmessage(EncryptMsgPtr);
	}
	else if (Message->GetTypeName() == "CheckRegParameterStatus") wrapper.set_allocated_checkregparameterstatus(static_cast<CheckRegParameterStatus*>(Message));
	else if (Message->GetTypeName() == "Registration") wrapper.set_allocated_registration(static_cast<Registration*>(Message));
	else if (Message->GetTypeName() == "Login") wrapper.set_allocated_login(static_cast<Login*>(Message));
	else if (Message->GetTypeName() == "EncryptedMessage") wrapper.set_allocated_encryptedmessage(static_cast<EncryptedMessage*>(Message));
	else if (Message->GetTypeName() == "KeyExchange") wrapper.set_allocated_keyexchange(static_cast<KeyExchange*>(Message));
	else delete Message;

	return wrapper;
}