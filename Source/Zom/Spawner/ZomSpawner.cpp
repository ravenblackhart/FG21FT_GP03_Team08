// Fill out your copyright notice in the Description page of Project Settings.


#include "ZomSpawner.h"


AZomSpawner::AZomSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	WaveIndex = 0;
}

void AZomSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZomSpawner::NextWave()
{
	if (!WaveConfigurations.IsValidIndex(WaveIndex))
	{
		return;
	}
	SpawnWave(WaveConfigurations[WaveIndex].CountToSpawn, WaveConfigurations[WaveIndex].SpawnInterval);
	GetWorldTimerManager().SetTimer(WaveTimer, this, &AZomSpawner::NextWave, WaveConfigurations[WaveIndex].TimeToNextWave, false);
	WaveIndex++;
}

void AZomSpawner::RepeatingSpawn()
{
	Spawn();
	
	TimesToRepeat--;
	if (TimesToRepeat <= 0)
	{
		GetWorldTimerManager().ClearTimer(IntervalTimer);
	}
}

void AZomSpawner::AbortSpawning()
{
	GetWorldTimerManager().ClearTimer(WaveTimer);
	GetWorldTimerManager().ClearTimer(IntervalTimer);
}

void AZomSpawner::ActivateSpawner()
{
	NextWave();
	OnSpawnerActivated();
}

void AZomSpawner::SpawnWave(int32 Amount, float SpawnInterval)
{
	TimesToRepeat = Amount;
	GetWorldTimerManager().SetTimer(IntervalTimer, this, &AZomSpawner::RepeatingSpawn, SpawnInterval, true, 0.f);
	
	OnWaveSpawned();
}

void AZomSpawner::Spawn()
{
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation(), GetActorRotation(), SpawnParameters);
	OnActorSpawned(SpawnedActor);
}
