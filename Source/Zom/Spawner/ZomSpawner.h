// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZomSpawner.generated.h"

USTRUCT(BlueprintType)
struct FWaveConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 CountToSpawn;
	UPROPERTY(EditAnywhere)
	float SpawnInterval;
	UPROPERTY(EditAnywhere)
	float TimeToNextWave;

	FWaveConfiguration()
	{
		CountToSpawn = 1;
		SpawnInterval = 2.0f;
		TimeToNextWave = 30.0f;
	}
};

UCLASS()
class ZOM_API AZomSpawner : public AActor
{
	GENERATED_BODY()

public:
	
	AZomSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ActorToSpawn;
	
	UPROPERTY(EditAnywhere)
	TArray<FWaveConfiguration> WaveConfigurations;
	
	UFUNCTION(BlueprintCallable)
	void SpawnWave(int32 Amount, float SpawnInterval);

	UFUNCTION(BlueprintImplementableEvent)
	void OnActorSpawned(AActor* SpawnedActor);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnWaveSpawned();

	UFUNCTION(BlueprintCallable)
	void AbortSpawning();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpawnerActivated();
	void ActivateSpawner();

protected:
	
	virtual void BeginPlay() override;

	void NextWave();
	
	void Spawn();

	void RepeatingSpawn();
	
	FTimerHandle IntervalTimer;
	FTimerHandle WaveTimer;

	int32 WaveIndex;
	int32 TimesToRepeat;

	FActorSpawnParameters SpawnParameters;
	
};
