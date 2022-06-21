// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionQueryParams.h"
#include "ZomMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZOM_API UZomMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UZomMovementComponent();
	
	FCollisionQueryParams TraceParams;
	FCollisionResponseParams Response;
	
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

	UFUNCTION(BlueprintCallable)
	void AddMovementFriction(const float FrictionPower);
private:
	// Input
	float MoveForwardBackwardInput;
	float MoveRightLeftInput;
	bool bIsPressingJumpInput;

	bool bIsTooSteep;
	float FixedTime;
	
	bool bIsCurrentlyJumping;
	bool bIsMovementDisabled;
	bool bIsOnGround;
	bool bIsFalling; 
	bool bQueueJump;
	float CurrentJumpTime;
	float CurrentCoyoteTimer;
	float CurrentJumpBufferTimer;
	FVector Velocity;

	// How fast the player moves
	UPROPERTY(EditAnywhere, Category = "Movement|Move")
	float Acceleration;

	// How steep of a surface the character can walk on.  
	UPROPERTY(EditAnywhere, Category = "Movement|Move")
	float MaxAngle;

	// How much friction to add when move forward
	UPROPERTY(EditAnywhere, Category = "Movement|Move")
	float ForwardMovementFriction;

	// How fast the player rotates when changing direction
	UPROPERTY(EditAnywhere, Category = "Movement|Move", meta=(ClampMin = "0.0", ClampMax = "50.0", UIMin = "0.0", UIMax = "50.0"))
	float CharacterRotationRate;

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
	float HoverLineTraceLength;

	// The height of edges we can walk over
	UPROPERTY(EditAnywhere, Category = "Movement|Hover")
	float HoverHeight;

	// How hard the player lands on the ground. The higher the number, the faster/harder you land
	UPROPERTY(EditAnywhere, Category = "Movement|Hover")
	float HoverSpringStrength;

	// Soften the land. The lower the number, the more spring-effect will be added
	UPROPERTY(EditAnywhere, Category = "Movement|Hover")
	float HoverSpringDamper;

	bool IsMoving() const;
	bool CanJump() const;
	void ResetJump();
	void TryAddRotation(float DeltaTime) const;
	void TryAddHovering(float DeltaTime);
	void TryAddJump(float DeltaTime);
	void TryAddMovement(float DeltaTime);
	void UpdateMovement(float DeltaTime);
	void OuterCollisionDetection(float DeltaTime);
	void HandleCollision(FHitResult& Hit, float& RemainingTime);
	void AddForce(const FVector& Force, float DeltaTime);
};
