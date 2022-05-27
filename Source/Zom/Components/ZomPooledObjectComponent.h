// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZomPooledObjectComponent.generated.h"

DECLARE_DELEGATE_OneParam(FOnPooledObjectDeactivated, class UZomPooledObjectComponent*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOM_API UZomPooledObjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UZomPooledObjectComponent();

	FOnPooledObjectDeactivated OnPooledObjectDeactivated;

	UFUNCTION(BlueprintCallable)
	void DeactivateObject();

	void ActivateObject();
	
protected:

	virtual void BeginPlay() override;
		
};
