#include "Networking/SocketObject.h"
#include "LoginSystem_Client/Protobufs/KeyMessages.pb.h"
#include "Handlers/MessageEncoder.h"
#include "Networking/SocketListener.h"


void USocketObject::InitSocket(FString ServerAddress, int32 ServerPort)
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP_SOCKET"), false);
	Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	
	FIPv4Address ServerIP;
	FIPv4Address::Parse(ServerAddress, ServerIP);
	
	Address->SetIp(ServerIP.Value);
	Address->SetPort(ServerPort);
	Socket->Connect(*Address);
	bIsConnection = Alive();

	FSocketListener::RunSocketListening();
}

void USocketObject::BeginDestroy()
{
	FSocketListener::Shutdown();
	if(Socket) Socket->Close();
	Super::BeginDestroy();
}

void USocketObject::Reconnect()
{
	Socket->Close();

	uint32 OutIP;
	Address->GetIp(OutIP);
	const FString ip = FString::Printf(TEXT("%d.%d.%d.%d"), 0xff & OutIP >> 24, 0xff & OutIP >> 16, 0xff & OutIP >> 8, 0xff & OutIP);

	InitSocket(ip, Address->GetPort());
}
 
bool USocketObject::Alive()
{
	return UMessageEncoder::SendAliveMessage();
}
 
bool USocketObject::ReadDelimitedFrom(google::protobuf::io::CodedInputStream* CodedInput, google::protobuf::MessageLite* Message)
{
	uint32_t Size;
	if (!CodedInput->ReadVarint32(&Size)) return false;
	
	const google::protobuf::io::CodedInputStream::Limit Limit = CodedInput->PushLimit(Size);
	
	if (!Message->MergeFromCodedStream(CodedInput) || !CodedInput->ConsumedEntireMessage()) return false;
	
	CodedInput->PopLimit(Limit);
	return true;
}