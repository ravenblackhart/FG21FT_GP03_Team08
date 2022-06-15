#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZomScoreCountingInterface.generated.h"

UINTERFACE(MinimalAPI)
class UZomScoreCountingInterface : public UInterface
{
	GENERATED_BODY()
};

class ZOM_API IZomScoreCountingInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Score")
	void EnemyKilled(const FName& ActorName);
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Score")
	int GetNumberOfKilledEnemies() const;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Score")
	int GetNumberOfSavedAllies() const;
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Score")
	int GetNumberOfKilledAllies() const;
	
};
