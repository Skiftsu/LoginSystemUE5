#include "Networking/SocketListener.h"
#include "Networking/SocketObject.h"
#include "Handlers/MessageDecoder.h"
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include <google/protobuf/io/coded_stream.h>
#include "LoginSystem_Client/Protobufs/KeyMessages.pb.h"
#include "Async/Async.h"

 
FSocketListener::FSocketListener()
{
	Thread = FRunnableThread::Create(this, TEXT("TCP_RECEIVER"), 0, TPri_BelowNormal);
	MessageDecoderObj = new MessageDecoder;
}
 
FSocketListener::~FSocketListener()
{
	delete MessageDecoderObj;
	delete Thread;
	Thread = nullptr;
}
 
bool FSocketListener::Init()
{
	bThreadRun = true;
	return bThreadRun;
}
 
uint32 FSocketListener::Run()
{
	while (bThreadRun)
	{
		if (!CheckServerStatus())
		{
			FPlatformProcess::Sleep(1.5f);
			continue;
		}
		
		FPlatformProcess::Sleep(0.2f); 
		if (!USocketObject::Socket) return 0;
			
		TArray<uint8> ReceivedData;
		uint32 Size;
		while (USocketObject::Socket->HasPendingData(Size))
		{
			ReceivedData.Init(FMath::Min(Size, 65507u), Size);
 
			int32 Read;
			USocketObject::Socket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
		}
		
		if (ReceivedData.Num() <= 0) continue;

		google::protobuf::io::ArrayInputStream ArrayInput(ReceivedData.GetData(), ReceivedData.Num());
		google::protobuf::io::CodedInputStream CodedInput(&ArrayInput);
 
		bool bProtoSize = true;
		while (bProtoSize)
		{
			TSharedPtr<Wrapper> Message(new Wrapper);
			bProtoSize = USocketObject::ReadDelimitedFrom(&CodedInput, Message.Get());
					
			AsyncTask(ENamedThreads::GameThread, [Message]{MessageDecoderObj->SendProtoToDecoder(Message.Get());});
		}
	}
	return 0;
}
 
FSocketListener* FSocketListener::RunSocketListening()
{
	if (!Runnable) Runnable = new FSocketListener();
	return Runnable;
}
 
void FSocketListener::Shutdown()
{
	bThreadRun = false;
 
	if (Runnable)
	{
		delete Runnable;
		Runnable = nullptr;
	}
}

bool FSocketListener::CheckServerStatus()
{
	if (!USocketObject::bIsConnection) USocketObject::Reconnect();
	else USocketObject::bIsConnection = USocketObject::Alive();
	return USocketObject::bIsConnection;
}
