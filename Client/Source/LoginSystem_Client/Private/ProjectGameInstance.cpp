#include "ProjectGameInstance.h"
#include "Networking/SocketObject.h"
#include "Config.h"

void UProjectGameInstance::Init()
{
	NewObject<USocketObject>(USocketObject::StaticClass())->InitSocket(FConfig::Address, FConfig::Port);
}