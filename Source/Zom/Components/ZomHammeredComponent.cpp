// Fill out your copyright notice in the Description page of Project Settings.


#include "Zom/Components/ZomHammeredComponent.h"

UZomHammeredComponent::UZomHammeredComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UZomHammeredComponent::ActivateHammered()
{
	OnHammered.Broadcast();
	UE_LOG(LogTemp, Warning, TEXT("ActivateHammered called."));
}

void UZomHammeredComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

