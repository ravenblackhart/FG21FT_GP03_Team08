#include "Zom/Player/ZomMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UZomMovementComponent::UZomMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;

	FixedTime = 1.f/60.f;

	// Movement controls 
	MoveForwardBackwardInput = 0.f;
	MoveRightLeftInput = 0.f;
	bIsPressingJumpInput = false;

	// Move
	Acceleration = 1000.f;
	ForwardMovementFriction = 4.5f;
	CharacterRotationRate = 10.f;
	bIsMovementDisabled = false;

	// Jump
	bIsCurrentlyJumping = false;
	MinJumpCurveTime = 0.f;
	MaxJumpCurveTime = 0.f;
	JumpForce = 5000.f;
	CurrentJumpTime = 0.f;
	JumpTime = 0.8f;
	CurrentCoyoteTimer = 0.f;
	CoyoteTime = 0.1f;
	CurrentJumpBufferTimer = 0.f;
	JumpBufferTime = 0.1f;

	// Hovering
	bIsOnGround = true;
	bIsFalling = false;
	GravityForce = 980.f;
	HoverLineTraceLength = 200.f;
	HoverHeight = 100.f;
	HoverSpringStrength = 1000.f;
	HoverSpringDamper = 1.5f;
	
	// Collision detection
	TraceParams.AddIgnoredActor(GetOwner());
	Response.CollisionResponse.SetResponse(ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	Response.CollisionResponse.SetResponse(ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
}

void UZomMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	JumpCurve->GetTimeRange(MinJumpCurveTime, MaxJumpCurveTime);
	CurrentJumpTime = MinJumpCurveTime;
}


void UZomMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*if (bIsMovementDisabled)
	{
		return;
	}*/

	TryAddHovering(DeltaTime);
	TryAddJump(DeltaTime);
	TryAddRotation(DeltaTime);
	TryAddMovement(DeltaTime);
	UpdateMovement(DeltaTime);
	OuterCollisionDetection(DeltaTime);
}

/**
 * @brief Set MoveForwardBackwardInput depending on player input
 * @param Value 1 Forward, -1 = Backward
 */
void UZomMovementComponent::SetMoveForwardBackwardInput(float Value)
{
	if (bIsMovementDisabled)
	{
		return;
	}

	MoveForwardBackwardInput = Value;
}

/**
 * @brief Set MoveRightLeftInput depending on player input
 * @param Value 1 Right, -1 = Left
 */
void UZomMovementComponent::SetMoveRightLeftInput(float Value)
{
	if (bIsMovementDisabled)
	{
		return;
	}

	MoveRightLeftInput = Value;
}

/**
 * @brief Sets if the player presses jump or not
 * @param bIsJumping True = is pressing Jump, False = is not pressing jump
 */
void UZomMovementComponent::SetJumpInput(bool bIsJumping)
{
	bIsPressingJumpInput = !bIsMovementDisabled && bIsJumping;
}

/**
 * @brief Gets the movement speed for the character
 * @return Speed of the forward movement
 */
float UZomMovementComponent::GetMovementSpeed() const
{
	return FVector::DotProduct(Velocity.ProjectOnTo(GetOwner()->GetActorForwardVector()), GetOwner()->GetActorForwardVector());
}

/**
 * @brief Checks if player is on ground
 * @return bIsOnGround
 */
bool UZomMovementComponent::IsOnGround() const
{
	return bIsOnGround;
}

/**
 * @brief Checks if jump force is being applied
 * @return bIsCurrentlyJumping
 */
bool UZomMovementComponent::IsJumping() const
{
	return bIsCurrentlyJumping;
}

/**
 * @brief Checks if player is in the air falling and not jumping anymore
 * @return bIsFalling
 */
bool UZomMovementComponent::IsFalling() const
{
	return bIsFalling;
}

/**
 * @brief Disables movement input
 */
void UZomMovementComponent::DisableMovement()
{
	Velocity = FVector::ZeroVector;
	bIsMovementDisabled = true;
	MoveForwardBackwardInput = 0;
	MoveRightLeftInput = 0;
}

/**
 * @brief Enables movement input
 */
void UZomMovementComponent::EnableMovement()
{
	Velocity = FVector::ZeroVector;
	bIsMovementDisabled = false;
}

/**
 * @brief Checks if the player wants to move
 * @return True if the player does any movement input else it returns false
 */
bool UZomMovementComponent::IsMoving() const
{
	return MoveForwardBackwardInput != 0.f || MoveRightLeftInput != 0.f;
}

/**
 * @brief Checks if the player can jump
 * @return True/False depending on if the player can jump
 */
bool UZomMovementComponent::CanJump() const
{
	return bIsOnGround && !bIsCurrentlyJumping;
}

/**
 * @brief Resets the jump state
 */
void UZomMovementComponent::ResetJump()
{
	bIsCurrentlyJumping = false;
	bIsFalling = true;
	CurrentJumpTime = 0.f;
	Velocity.Z = 0.f;
}

/**
 * @brief Rotates the player when changing direction with the help of WASD and the camera
 * @param DeltaTime
 */
void UZomMovementComponent::TryAddRotation(float DeltaTime) const
{
	if (!IsMoving())
	{
		return;
	}

	FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector CameraLocationDir = GetOwner()->GetActorLocation() - CameraLocation;
	CameraLocationDir.Normalize();

	FVector MovementDirection = FVector(MoveForwardBackwardInput, MoveRightLeftInput, 1.f);
	FRotator YawRotation = FRotator(0.f, CameraLocationDir.Rotation().Yaw + MovementDirection.Rotation().Yaw, 0.f);

	GetOwner()->SetActorRotation(FMath::RInterpTo(GetOwner()->GetActorRotation(), YawRotation, DeltaTime, CharacterRotationRate));
}

/**
 * @brief Makes the player hover above ground, so the player can walk up/down of slopes
 */
void UZomMovementComponent::TryAddHovering(float DeltaTime)
{
	// Push Down
	AddForce(FVector::DownVector * GravityForce, DeltaTime);

	// Sweep data
	const FVector Start = GetOwner()->GetActorLocation();
	const FVector End = Start + (FVector::DownVector * HoverLineTraceLength);
	FHitResult Hit;
	
	// Push Up (On Ground)
	// TODO: Make sure its the ground that was hit
	if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeCapsule(30.f, 20.f), TraceParams, Response))
	{
		const float HitDist = FVector::Distance(Start, Hit.Location);
		const float Difference = HitDist - HoverHeight;
		const float CappedDeltaTime = FMath::Min(DeltaTime, FixedTime);
		const float SpringHoverForce = (Difference * HoverSpringStrength) + (Velocity.Z * HoverSpringDamper);
		const float CollisionDotProduct = FMath::Abs(GetOwner()->GetActorUpVector().Dot(Hit.Normal));

		// If the collision is not from beneath, it should be ignored.
		if (CollisionDotProduct < 0.5f)
		{
			return;
		}

		if (HitDist <= HoverHeight && !bIsCurrentlyJumping)
		{
			CurrentCoyoteTimer = 0.f;
			bIsOnGround = true;
			bIsFalling = false;
		}
		else if (!bIsCurrentlyJumping && !bIsOnGround)
		{
			bIsFalling = true;
		}

		if (bIsOnGround)
		{
			AddForce(FVector::DownVector * SpringHoverForce, CappedDeltaTime);
		}
	}
	else if (!bIsCurrentlyJumping)
	{
		if (bIsOnGround)
		{
			CurrentCoyoteTimer += DeltaTime;

			if (CurrentCoyoteTimer >= CoyoteTime)
			{
				CurrentCoyoteTimer = 0.f;
				bIsOnGround = false;
			}
		} else
		{
			bIsFalling = true;
		}
	}
}

/**
 * @brief Check if the player is jumping and adds upward force
 * @param DeltaTime
 */
void UZomMovementComponent::TryAddJump(float DeltaTime)
{
	// Initialize the jump
	if (CanJump() && bIsPressingJumpInput)
	{
		CurrentJumpBufferTimer = 0;
		CurrentCoyoteTimer = 0;
		bIsCurrentlyJumping = true;
		bIsOnGround = false;
		bIsPressingJumpInput = false;
		Velocity.Z = 0.f;
	}
	else if (bIsPressingJumpInput && !bQueueJump) // Queuing a jump
	{
		bQueueJump = true;
	}

	// Jump buffer 
	if (bQueueJump)
	{
		CurrentJumpBufferTimer += DeltaTime;

		if (CurrentJumpBufferTimer >= JumpBufferTime)
		{
			CurrentJumpBufferTimer = 0;
			bIsPressingJumpInput = false;
			bQueueJump = false;
		}
	}

	// Jumping
	if (bIsCurrentlyJumping)
	{
		CurrentJumpTime += DeltaTime;

		if (CurrentJumpTime >= JumpTime || bIsFalling && bIsOnGround)
		{
			bIsCurrentlyJumping = false;
			bIsFalling = !bIsOnGround;
			CurrentJumpTime = 0;
			return;
		}

		if (bIsCurrentlyJumping)
		{
			const float JumpPercentage = CurrentJumpTime / JumpTime;
			const float JumpCurveValue = JumpCurve->GetFloatValue(JumpPercentage * MaxJumpCurveTime);

			AddForce(FVector::UpVector * (JumpForce * JumpCurveValue), DeltaTime);
		}
	}
}

/**
 * @brief Moves the player forward direction, to go other directions the player has to rotate
 * @param DeltaTime
 */
void UZomMovementComponent::TryAddMovement(float DeltaTime)
{
	// Add forward force and friction
	AddForce(GetOwner()->GetActorForwardVector() * (Acceleration * IsMoving()), DeltaTime);
	AddMovementFriction(ForwardMovementFriction);
}

void UZomMovementComponent::UpdateMovement(float DeltaTime)
{
	float RemainingTime = DeltaTime;
	int Interations = 0;

	while (RemainingTime > 0.f && ++Interations < 5)
	{
		FHitResult Hit;
		GetOwner()->AddActorWorldOffset(Velocity * RemainingTime, true, &Hit);

		if (Hit.bBlockingHit)
		{
			HandleCollision(Hit, RemainingTime);
		} else 
		{
			break;
		}
	}
}

/**
 * @brief Checks if other WorldDynamics things is colliding while not moving (For example the battle ram)
 * If true, CheckCollsion will handle depenetration problems
 * @param DeltaTime
 */
void UZomMovementComponent::OuterCollisionDetection(float DeltaTime)
{
	TArray<FHitResult> Hits;
	const FVector Start = GetOwner()->GetActorLocation() - GetOwner()->GetActorUpVector() * 10.f;
	const FVector End = GetOwner()->GetActorLocation() + GetOwner()->GetActorUpVector() * 10.f;
	
	GetWorld()->SweepMultiByChannel(Hits, Start, End, FQuat::Identity, ECC_WorldDynamic, FCollisionShape::MakeCapsule(20.f, 25.f), TraceParams);

	for (auto Hit : Hits)
	{
		if (Hit.bBlockingHit)
		{
			HandleCollision(Hit, DeltaTime);
		}
	}
}

/**
 * @brief 
 * @param Hit 
 * @param a 
 */
void UZomMovementComponent::HandleCollision(FHitResult& Hit, float& RemainingTime)
{
	// Check if the character is jumping and got hit in the head
	if (GetOwner()->GetActorUpVector().Dot(Hit.Normal) > 0.5f && bIsCurrentlyJumping)
	{
		ResetJump();
	}

	// If first time overlapping collision
	if (Hit.bStartPenetrating)
	{
		FVector LocationDirection = (Hit.GetActor()->GetActorLocation() - Hit.Location);
		LocationDirection.Normalize();
		LocationDirection.Z = 0.f;

		const float CollisionDirection = FMath::Abs(Hit.GetActor()->GetActorForwardVector().Dot(LocationDirection));
		const float DepenetrationOffset = 0.05f * (1.f + (1.f - CollisionDirection));
		
		FVector Depenetration = Hit.Normal * (Hit.PenetrationDepth + DepenetrationOffset);
		Depenetration.Z = 0;

		GetOwner()->AddActorWorldOffset(Depenetration);
	}
	else
	{
		// If one direction is colliding, block it and continue move other direction
		// For example X direction is blocking, keep moving Y direction 
		FVector PlaneVector = FVector::VectorPlaneProject(Velocity, Hit.Normal);
		Velocity = FVector(PlaneVector.X, PlaneVector.Y, Velocity.Z);
		RemainingTime -= RemainingTime * Hit.Time;
	}
}

/**
 * @brief Adds force incrementally
 * @param Force Vector for which direction and force to add
 */
void UZomMovementComponent::AddForce(const FVector& Force, float DeltaTime)
{
	Velocity += Force * DeltaTime;
}

 /**
  * @brief Adjust and adds friction the forward movement force
  * @param FrictionPower How much of the forward velocity will be removed per second.
  */
void UZomMovementComponent::AddMovementFriction(UPARAM() float FrictionPower)
{
	FVector ForwardMovement = Velocity.ProjectOnTo(GetOwner()->GetActorForwardVector());
	
	ForwardMovement -= (ForwardMovement * FrictionPower * GetWorld()->GetDeltaSeconds());
	ForwardMovement.Z = Velocity.Z;

	Velocity = ForwardMovement;
}