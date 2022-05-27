// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CombatAffiliationType.generated.h"

UENUM(BlueprintType)
enum class ECombatAffiliationType : uint8
{
	NETURAL UMETA(DisplayName = "Netural"),
	ALLY UMETA(DisplayName = "Ally"),
	ENEMY UMETA(DisplayName = "Enemy"),
};