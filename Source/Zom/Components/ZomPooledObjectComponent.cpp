// Fill out your copyright notice in the Description page of Project Settings.


#include "Zom/Components/ZomPooledObjectComponent.h"

UZomPooledObjectComponent::UZomPooledObjectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UZomPooledObjectComponent::DeactivateObject()
{
	GetOwner()->SetActorHiddenInGame(true);
	GetOwner()->SetActorTickEnabled(false);
	OnPooledObjectDeactivated.ExecuteIfBound(this);
}

void UZomPooledObjectComponent::ActivateObject()
{
	GetOwner()->SetActorHiddenInGame(false);
	GetOwner()->SetActorTickEnabled(true);
}

void UZomPooledObjectComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

