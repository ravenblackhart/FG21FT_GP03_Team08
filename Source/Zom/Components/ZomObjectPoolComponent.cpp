// Fill out your copyright notice in the Description page of Project Settings.

#include "Zom/Components/ZomObjectPoolComponent.h"
#include "Zom/Components/ZomPooledObjectComponent.h"

UZomObjectPoolComponent::UZomObjectPoolComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PoolSize = 10;
}

AActor* UZomObjectPoolComponent::SpawnPooledObject()
{
	UZomPooledObjectComponent* Comp;
	if (QueuedComponents.Dequeue(Comp))
	{
		Comp->ActivateObject();
		return Comp->GetOwner();
	}
	UE_LOG(LogTemp, Warning, TEXT("Reached pool limit."));
	return nullptr;
}

void UZomObjectPoolComponent::ReturnToPool(UZomPooledObjectComponent* Component)
{
	QueuedComponents.Enqueue(Component);
}

void UZomObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (!PooledObjectClass)
	{
		return;
	}

	if (UWorld* World = GetWorld())
	{
		for (int i = 0; i < PoolSize; i++)
		{
			if (AActor* SpawnedActor = World->SpawnActor<AActor>(PooledObjectClass))
			{
				SpawnedActor->SetActorTickEnabled(false);
				SpawnedActor->SetActorHiddenInGame(true);
				if (UZomPooledObjectComponent* Component = SpawnedActor->FindComponentByClass<UZomPooledObjectComponent>())
				{
					Component->OnPooledObjectDeactivated.BindUObject(this, &UZomObjectPoolComponent::ReturnToPool);
					QueuedComponents.Enqueue(Component);
				}
			}
		}
	}
	
}

