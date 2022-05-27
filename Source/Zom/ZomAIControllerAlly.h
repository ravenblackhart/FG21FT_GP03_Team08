#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "ZomAIControllerAlly.generated.h"

/**
 *
 */
UCLASS(Abstract)
class ZOM_API AZomAIControllerAlly : public AAIController
{
	GENERATED_BODY()
public:
	AZomAIControllerAlly();

	UFUNCTION()
	void OnPerceptionUpdated(AActor* actor, FAIStimulus stimulus);

	UPROPERTY(BlueprintReadWrite)
	AActor* payload = nullptr;

protected:
	void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	UAIPerceptionComponent* percComp = nullptr;

	UPROPERTY()
	UBlackboardComponent* bbComp = nullptr;
};