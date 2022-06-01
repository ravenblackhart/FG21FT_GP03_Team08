// Fill out your copyright notice in the Description page of Project Settings.


#include "ZomSpawnerManager.h"
#include "ZomSpawner.h"


AZomSpawnerManager::AZomSpawnerManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AZomSpawnerManager::ActivateSpawners()
{
	for (AZomSpawner* Spawner : Spawners)
	{
		Spawner->ActivateSpawner();
	}
}

void AZomSpawnerManager::DisableSpawners()
{
	for (AZomSpawner* Spawner : Spawners)
	{
		Spawner->AbortSpawning();
	}
}

void AZomSpawnerManager::BeginPlay()
{
	Super::BeginPlay();
	
}
