// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZomRegionDoorComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOM_API UZomRegionDoorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UZomRegionDoorComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Trigger();
		
};
