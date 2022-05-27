// Fill out your copyright notice in the Description page of Project Settings.

#include "Zom/Components/ZomBatteredComponent.h"

UZomBatteredComponent::UZomBatteredComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UZomBatteredComponent::ActivateBattered()
{
	OnBattered.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("ActivateBattered called."));
}

void UZomBatteredComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

