// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZomLastDoorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOM_API UZomLastDoorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UZomLastDoorComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Trigger();
		
};
