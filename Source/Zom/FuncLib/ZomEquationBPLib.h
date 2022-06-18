// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ZomEquationBPLib.generated.h"

UCLASS()
class UZomEquationBPLib : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UZomEquationBPLib();

    UFUNCTION(BlueprintCallable)
    static float RamEquation(int n);
};