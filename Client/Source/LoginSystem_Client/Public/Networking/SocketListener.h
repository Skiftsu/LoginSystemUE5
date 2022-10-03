#pragma once
 
#include "Runtime/Core/Public/HAL/Runnable.h"
 
class LOGINSYSTEM_CLIENT_API FSocketListener : public FRunnable
{
	FRunnableThread* Thread;
	static inline FSocketListener* Runnable;
	static inline bool bThreadRun = false;
 
public:
	FSocketListener();
	virtual ~FSocketListener() override;
 
	virtual bool Init() override;
	virtual uint32 Run() override;
	static FSocketListener* RunSocketListening();
	static void Shutdown();
	bool CheckServerStatus();

	static inline class MessageDecoder* MessageDecoderObj;
};