#include "Zom/Allies/ZomAlly.h"

#include "Kismet/GameplayStatics.h"
#include "Zom/ZomAIControllerAlly.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Zom/ZomGameModeBase.h"

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
	Super::BeginPlay();

	Cast<AZomGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->AvailableAllies[region]++;
}

void AZomAlly::Tick(float DeltaTime)
{

}
