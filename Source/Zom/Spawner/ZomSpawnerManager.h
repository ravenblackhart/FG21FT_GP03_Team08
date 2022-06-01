// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZomSpawnerManager.generated.h"

class AZomSpawner;

UCLASS()
class ZOM_API AZomSpawnerManager : public AActor
{
	GENERATED_BODY()

public:
	
	AZomSpawnerManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AZomSpawner*> Spawners;

	UFUNCTION(BlueprintCallable)
	void ActivateSpawners();
	
	UFUNCTION(BlueprintCallable)
	void DisableSpawners();
	
protected:
	
	virtual void BeginPlay() override;
	
};
