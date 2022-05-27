// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZomObjectPoolComponent.generated.h"

class UZomPooledObjectComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOM_API UZomObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UZomObjectPoolComponent();

	UFUNCTION(BlueprintCallable, Category = "Pool")
	AActor* SpawnPooledObject();

	UPROPERTY(EditAnywhere, Category = "Pool")
	TSubclassOf<AActor> PooledObjectClass;
	
	UPROPERTY(EditAnywhere, Category = "Pool")
	int32 PoolSize;

	UFUNCTION()
	void ReturnToPool(UZomPooledObjectComponent* Component);
	
protected:
	
	virtual void BeginPlay() override;

	TQueue<UZomPooledObjectComponent*> QueuedComponents;
	
};
