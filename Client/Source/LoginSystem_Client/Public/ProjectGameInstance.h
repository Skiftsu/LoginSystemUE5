#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectGameInstance.generated.h"


UCLASS()
class LOGINSYSTEM_CLIENT_API UProjectGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;
};
