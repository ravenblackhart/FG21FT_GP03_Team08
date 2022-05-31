// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZomSpawner.generated.h"

UCLASS()
class ZOM_API AZomSpawner : public AActor
{
	GENERATED_BODY()

public:
	
	AZomSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorToSpawn;

	UFUNCTION(BlueprintCallable)
	void SpawnWave(int32 Amount, float SpawnInterval);

	UFUNCTION(BlueprintImplementableEvent)
	void OnActorSpawned(AActor* SpawnedActor);

	UFUNCTION(BlueprintCallable)
	void AbortSpawning();

protected:
	
	virtual void BeginPlay() override;

	void Spawn();

	void RepeatingSpawn();
	
	FTimerHandle IntervalTimer;
	
	int32 TimesToRepeat;

	FActorSpawnParameters SpawnParameters;
	
};
