#pragma once
 
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Networking.h"
#include "SocketSubsystem.h"

#include <google/protobuf/message.h>

#include "SocketObject.generated.h"
 

UCLASS()
class LOGINSYSTEM_CLIENT_API USocketObject : public UObject
{
	GENERATED_BODY()
	
public:
	static inline FSocket* Socket;
	static inline TSharedPtr<FInternetAddr> Address;
	static inline bool bIsConnection;
	
	static void Reconnect();
	static bool Alive();
 
	static bool ReadDelimitedFrom(google::protobuf::io::CodedInputStream* CodedInput, google::protobuf::MessageLite* Message);
	static void InitSocket(FString ServerAddress, int32 ServerPort);
	virtual void BeginDestroy() override;
};