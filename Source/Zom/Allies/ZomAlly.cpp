#include "Zom/Allies/ZomAlly.h"

#include "Kismet/GameplayStatics.h"
#include "Zom/ZomAIControllerAlly.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Zom/ZomGameModeBase.h"
#include "UObject/UObjectBaseUtility.h"

AZomAlly::AZomAlly()
{
	bActive = false;
	bPayloaded = false;
	bAlive = true;
	bRecentlyHit = false;
	region = -1;

	PrimaryActorTick.bCanEverTick = true;
}

void AZomAlly::OnHearingPlayer_Implementation()
{
	if (!bActive && !bPayloaded && bAlive)
		bActive = true;
}

void AZomAlly::ActivateAlly_Implementation()
{
	if (!bPayloaded && bActive && bAlive)
	{
		bPayloaded = true;
		UAIBlueprintHelperLibrary::GetAIController(this)->GetBrainComponent()->StopLogic(TEXT("Attached to payload"));
	}
}

void AZomAlly::BeginPlay()
{
	if (region < 0)
	{
		region = 0;
		FString name = GetName();
		UE_LOG(LogTemp, Warning, TEXT("Region not set on ally: %s"), *name);
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Region not set on ally, see LogTemp warning"));
	}

	Super::BeginPlay();
}

void AZomAlly::Tick(float DeltaTime)
{

}
