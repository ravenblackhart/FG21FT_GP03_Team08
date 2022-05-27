// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZomHealthComponent.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class ZOM_API UZomHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UZomHealthComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int Health = -1;

};
