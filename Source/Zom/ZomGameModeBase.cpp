// Copyright Epic Games, Inc. All Rights Reserved.


#include "ZomGameModeBase.h"

AZomGameModeBase::AZomGameModeBase()
{
	AvailableAllies.Init(0, 3);
	RespawnPoints.Init(nullptr, 3);
}

void AZomGameModeBase::RegisterSpawner(AActor* respawner, int region)
{
	if (RespawnPoints[region] == nullptr)
		RespawnPoints[region] = respawner;
	else
	{
		FString name = GetName();
		UE_LOG(LogTemp, Warning, TEXT("Region not set on respawner: %s"), *name);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Region not set on respawner, see LogTemp warning"));
	}
}
