// Fill out your copyright notice in the Description page of Project Settings.


#include "ZomAIControllerAlly.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZomGameModeBase.h"

AZomAIControllerAlly::AZomAIControllerAlly()
{
	percComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	percComp->OnTargetPerceptionUpdated.AddDynamic(this, &AZomAIControllerAlly::OnPerceptionUpdated);
}

void AZomAIControllerAlly::OnPerceptionUpdated(AActor* actor, FAIStimulus stimulus)
{
	if (actor == Cast<AZomGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->BatteringRam)
	{
		if (bbComp == nullptr)
			bbComp = GetBlackboardComponent();

		if (stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("See Ram!"));
			bbComp->SetValueAsObject("Payload", actor);
			//bbComp->SetValueAsBool("Alert", true);
			//bbComp->ClearValue("LastSpotPos");
			return;
		}

		//bbComp->SetValueAsVector("LastSpotLos", character->GetActorLocation());
		//bbComp->SetValueAsBool("HasLOS", false);
	}
}

void AZomAIControllerAlly::BeginPlay()
{
	Super::BeginPlay();

	//playerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	bbComp = GetBlackboardComponent();
	//bbComp->SetValueAsObject("Player", playerRef);
}
