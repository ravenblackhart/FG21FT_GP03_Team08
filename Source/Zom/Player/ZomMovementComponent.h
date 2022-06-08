// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ZomMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOM_API UZomMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UZomMovementComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetMoveForwardBackwardInput(float Value);
	void SetMoveRightLeftInput(float Value);
	void SetJumpInput(bool bIsJumping);
	
	UFUNCTION(BlueprintPure)
	float GetMovementSpeed() const;
	
	UFUNCTION(BlueprintPure)
	bool IsOnGround() const;
	
	UFUNCTION(BlueprintPure)
	bool IsJumping() const;
	
	UFUNCTION(BlueprintPure)
	bool IsFalling() const;

	UFUNCTION(BlueprintCallable)
	void DisableMovement();
	
	UFUNCTION(BlueprintCallable)
	void EnableMovement();
private:
	// Input
	float MoveForwardBackwardInput;
	float MoveRightLeftInput;
	bool bIsPressingJumpInput;
	
	bool bIsCurrentlyJumping;
	bool bIsMovementDisabled;
	bool bIsOnGround;
	bool bIsFalling;
	bool bQueueJump;
	float MinJumpCurveTime;
	float CurrentJumpTime;
	float CurrentCoyoteTimer;
	float CurrentJumpBufferTimer;
	float MaxJumpCurveTime;
	FVector Velocity;

	// How fast the player moves
	UPROPERTY(EditAnywhere, Category = "Movement|Move")
	float Acceleration;

	// How much friction to add when move forward
	UPROPERTY(EditAnywhere, Category = "Movement|Move")
	float ForwardMovementFriction;

	// How fast the player rotates when changing direction
	UPROPERTY(EditAnywhere, Category = "Movement|Move", meta=(ClampMin = "0.0", ClampMax = "10.0", UIMin = "0.0", UIMax = "10.0"))
	float CharacterRotationRate;

	// How the jump force should be applied
	UPROPERTY(EditAnywhere, Category = "Movement|Jump")
	UCurveFloat* JumpCurve;

	// How much force to push up when jump
	UPROPERTY(EditAnywhere, Category = "Movement|Jump")
	float JumpForce;

	// How long the jump force should be applied
	UPROPERTY(EditAnywhere, Category = "Movement|Jump")
	float JumpTime;

	// How long you have to jump when falling from an edge
	UPROPERTY(EditAnywhere, Category = "Movement|Jump")
	float CoyoteTime;

	// How long we save jump input before landing
	UPROPERTY(EditAnywhere, Category = "Movement|Jump")
	float JumpBufferTime;

	// How much force the player is being pushed down
	UPROPERTY(EditAnywhere, Category = "Movement|Hover")
	float GravityForce;

	// How far down from the characters center we track the ground
	// The further down, the more spring-effect we can add when the player lands
	UPROPERTY(EditAnywhere, Category = "Movement|Hover")
	float LineTraceLength;

	// The height of edges we can walk over
	UPROPERTY(EditAnywhere, Category = "Movement|Hover")
	float RideHeight;

	// How hard the player lands on the ground. The higher the number, the faster/harder you land
	UPROPERTY(EditAnywhere, Category = "Movement|Hover")
	float RideSpringStrength;

	// Soften the land. The lower the number, the more spring-effect will be added
	UPROPERTY(EditAnywhere, Category = "Movement|Hover")
	float RideSpringDamper;

	bool IsMoving() const;
	bool CanJump() const;
	void Rotate(float DeltaTime) const;
	void Hover(float DeltaTime);
	void Jump(float DeltaTime);
	void Move(float DeltaTime);
	void AddForce(const FVector& Force, float DeltaTime);
	void AddMovementFriction(const FVector& ForwardVector);
};
