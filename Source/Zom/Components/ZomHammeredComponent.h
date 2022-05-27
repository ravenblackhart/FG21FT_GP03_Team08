// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZomHammeredComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHammered);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOM_API UZomHammeredComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UZomHammeredComponent();

	UFUNCTION(BlueprintCallable)
	void ActivateHammered();

	UPROPERTY(BlueprintAssignable)
	FOnHammered OnHammered;
	
protected:

	virtual void BeginPlay() override;
		
};
