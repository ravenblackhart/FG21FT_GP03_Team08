// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Zom/Enums/CombatAffiliationType.h"
#include "ZomCombatInterface.generated.h"

UINTERFACE(BlueprintType)
class UZomCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class ZOM_API IZomCombatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Combat")
	bool IsTargetable();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Combat")
	ECombatAffiliationType GetCombatAffiliation();
};
