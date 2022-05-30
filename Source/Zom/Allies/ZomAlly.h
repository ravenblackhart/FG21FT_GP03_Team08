#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZomAlly.generated.h"

UCLASS(Abstract)
class ZOM_API AZomAlly : public ACharacter
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		bool bActive;

	UPROPERTY(BlueprintReadWrite)
		bool bPayloaded;

	UPROPERTY(BlueprintReadWrite)
		bool bAlive;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int region;

	AZomAlly();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHearingPlayer();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ActivateAlly();

protected:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	bool bRecentlyHit;
};