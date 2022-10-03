#pragma once

#include "CoreMinimal.h"
#include "google/protobuf/message.h"
#include "UObject/Object.h"
#include "MessageEncoder.generated.h"


UCLASS()
class LOGINSYSTEM_CLIENT_API UMessageEncoder : public UObject
{
	GENERATED_BODY()
	
public:
	static bool Send(google::protobuf::Message* Message, const bool Encrypt = false);
	static bool SendAliveMessage();

private:
	static bool SendProtobufMessage(const class Wrapper Message);
	static bool EncryptProtobufMessage(const google::protobuf::Message* Message, class EncryptedMessage* EncryptMessage);
	static Wrapper WrapMessage(google::protobuf::Message* Message, const bool Encrypt = false);
};
	