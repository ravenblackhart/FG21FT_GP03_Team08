// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZomTriggerInterface.generated.h"

UINTERFACE(BlueprintType)
class UZomTriggerInterface : public UInterface
{
	GENERATED_BODY()
};

class ZOM_API IZomTriggerInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnTriggered();
	
};
