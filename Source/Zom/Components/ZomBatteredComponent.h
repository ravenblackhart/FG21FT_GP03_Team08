// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZomBatteredComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBattered);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOM_API UZomBatteredComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UZomBatteredComponent();

	UFUNCTION(BlueprintCallable)
	void ActivateBattered();

	UPROPERTY(EditInstanceOnly)
	int32 Health; 
	
	UPROPERTY(BlueprintAssignable)
	FOnBattered OnBattered;
	
protected:

	virtual void BeginPlay() override;
		
};
