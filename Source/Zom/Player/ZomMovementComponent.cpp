#include "Zom/Player/ZomMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UZomMovementComponent::UZomMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Movement controls 
	MoveForwardBackwardInput = 0.f;
	MoveRightLeftInput = 0.f;
	bIsPressingJumpInput = false;

	// Move
	Acceleration = 1000.f;
	ForwardMovementFriction = 4.5f;
	CharacterRotationRate = 10.f;

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
	bIsOnGround = false;
	bIsFalling = false;
	GravityForce = 980.f;
	LineTraceLength = 200.f;
	RideHeight = 100.f;
	RideSpringStrength = 1000.f;
	RideSpringDamper = 1.5f;
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
	
	Hover(DeltaTime);
	Jump(DeltaTime);
	Rotate(DeltaTime);
	Move(DeltaTime);
}

/**
 * @brief Set MoveForwardBackwardInput depending on player input
 * @param Value 1 Forward, -1 = Backward
 */
void UZomMovementComponent::SetMoveForwardBackwardInput(float Value)
{
	MoveForwardBackwardInput = Value;
}

/**
 * @brief Set MoveRightLeftInput depending on player input
 * @param Value 1 Right, -1 = Left
 */
void UZomMovementComponent::SetMoveRightLeftInput(float Value)
{
	MoveRightLeftInput = Value;
}

/**
 * @brief Sets if the player presses jump or not
 * @param bIsJumping True = is pressing Jump, False = is not pressing jump
 */
void UZomMovementComponent::SetJumpInput(bool bIsJumping)
{
	bIsPressingJumpInput = bIsJumping;
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
 * @brief Rotates the player when changing direction with the help of WASD and the camera
 * @param DeltaTime 
 */
void UZomMovementComponent::Rotate(float DeltaTime) const
{
	if (!IsMoving())
	{
		return;
	}

	AActor* Owner = GetOwner();

	FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector CameraLocationDir = Owner->GetActorLocation() - CameraLocation;
	CameraLocationDir.Normalize();

	FVector MovementDirection = FVector(MoveForwardBackwardInput, MoveRightLeftInput, 1.f);
	FRotator YawRotation = FRotator(0.f, CameraLocationDir.Rotation().Yaw + MovementDirection.Rotation().Yaw, 0.f);

	Owner->SetActorRotation(FMath::RInterpTo(Owner->GetActorRotation(), YawRotation, DeltaTime, CharacterRotationRate));
}

/**
 * @brief Makes the player hover above ground, so the player can walk up/down of slopes
 */
void UZomMovementComponent::Hover(float DeltaTime)
{
	// Push Down
	AddForce(FVector::DownVector * GravityForce);

	// Sweep data
	AActor* Owner = GetOwner();
	FVector Start = Owner->GetActorLocation();
	FVector End = Start + (FVector::DownVector * LineTraceLength);
	FHitResult Hit;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(Owner);
	
	// Push Up (On Ground)
	// TODO: Make sure its the ground that was hit
	if (GetWorld()->SweepSingleByChannel(Hit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(30.0f), TraceParams))
	{
		float HitDist = FVector::Distance(Start, Hit.Location);
		float Difference = HitDist - RideHeight;
		float SpringHoverForce = (Difference * RideSpringStrength) + (Velocity.Z * RideSpringDamper);

		if (HitDist <= RideHeight && !bIsCurrentlyJumping)
		{
			CurrentCoyoteTimer = 0.f;
			bIsOnGround = true;
		}
		
		if (bIsOnGround)
		{
			AddForce(FVector::DownVector * SpringHoverForce);
		}
	} else if (!bIsCurrentlyJumping)
	{
		if (bIsOnGround)
		{
			CurrentCoyoteTimer += DeltaTime;

			if (CurrentCoyoteTimer >= CoyoteTime)
			{
				CurrentCoyoteTimer = 0.f;
				bIsOnGround = false;
			}
		}
	}
}

/**
 * @brief Check if the player is jumping and adds upward force
 * @param DeltaTime 
 */
void UZomMovementComponent::Jump(float DeltaTime)
{
	AActor* Owner = GetOwner();

	// Initialize the jump
	if (CanJump() && bIsPressingJumpInput)
	{
		CurrentJumpBufferTimer = 0;
		CurrentCoyoteTimer = 0;
		bIsCurrentlyJumping = true;
		bIsOnGround = false;
		bIsPressingJumpInput = false;
	} else if (bIsPressingJumpInput && !bQueueJump) // Queuing a jump
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
			bIsFalling = false;
			bIsCurrentlyJumping = false;
			CurrentJumpTime = 0;
			return;
		}

		if (bIsCurrentlyJumping)
		{
			float JumpPercentage = CurrentJumpTime / JumpTime;
			float JumpCurveValue = JumpCurve->GetFloatValue(JumpPercentage * MaxJumpCurveTime);

			bIsFalling = JumpPercentage > 1;

			if (Velocity.Z < 0)
			{
				 Velocity.Z = 0;
			}

			AddForce(FVector::UpVector * (JumpForce * JumpCurveValue));
		}
	}
}

/**
 * @brief Moves the player forward direction, to go other directions the player has to rotate
 * @param DeltaTime
 */
void UZomMovementComponent::Move(float DeltaTime)
{
	AActor* Owner = GetOwner();

	// Add forward force and friction
	AddForce(Owner->GetActorForwardVector() * Acceleration * IsMoving());
	AddMovementFriction(Owner->GetActorForwardVector());
	
	float RemainingTime = DeltaTime;
	int Interations = 0;

	while (RemainingTime > 0.f && ++Interations < 10)
	{
		FHitResult Hit;
		Owner->AddActorWorldOffset(Velocity * RemainingTime, true, &Hit);

		// We hitted something while we moved
		if (Hit.bBlockingHit)
		{
			// If first time overlapping collision
			if (Hit.bStartPenetrating)
			{
				// Depenetrating the overlap
				Owner->AddActorWorldOffset(Hit.Normal * (Hit.PenetrationDepth + 0.1f));
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
		else
		{
			break;
		}
	}
}

/**
 * @brief Adds force incrementally
 * @param Force Vector for which direction and force to add
 */void UZomMovementComponent::AddForce(const FVector& Force)
{
	Velocity += Force * GetWorld()->GetDeltaSeconds();
}

/**
 * @brief Adjust and adds friction the forward movement force
 * @param ProjectOnToDirection 
 */
void UZomMovementComponent::AddMovementFriction(const FVector& ProjectOnToDirection)
{
	FVector ForwardMovement = Velocity.ProjectOnTo(ProjectOnToDirection);
	ForwardMovement -= (ForwardMovement * ForwardMovementFriction * GetWorld()->GetDeltaSeconds());
	ForwardMovement.Z = Velocity.Z;

	Velocity = ForwardMovement;
}