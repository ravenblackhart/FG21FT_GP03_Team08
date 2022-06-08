// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ZomGameModeBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ZOM_API AZomGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AZomGameModeBase();
	
	UPROPERTY(BlueprintReadWrite)
	AActor* BatteringRam = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TArray<int> AvailableAllies;

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> RespawnPoints;

	UPROPERTY(BlueprintReadWrite)
	int ActiveRegion = 0;

	UFUNCTION(BlueprintCallable)
	void RegisterSpawner(AActor* spawner, int region);
};
