// Fill out your copyright notice in the Description page of Project Settings.


#include "ZomSpawner.h"


AZomSpawner::AZomSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
}

void AZomSpawner::BeginPlay()
{
	Super::BeginPlay();
	
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
	GetWorldTimerManager().ClearTimer(IntervalTimer);
}

void AZomSpawner::SpawnWave(int32 Amount, float SpawnInterval)
{
	TimesToRepeat = Amount;
	GetWorldTimerManager().SetTimer(IntervalTimer, this, &AZomSpawner::RepeatingSpawn, SpawnInterval, true, 0.f);
}

void AZomSpawner::Spawn()
{
	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorToSpawn, GetActorLocation(), GetActorRotation(), SpawnParameters);
	OnActorSpawned(SpawnedActor);
}
