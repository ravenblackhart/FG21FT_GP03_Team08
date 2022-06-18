#include "Zom/Player/ZomPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Zom/Player/ZomMovementComponent.h"

AZomPlayer::AZomPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	bAllowTickBeforeBeginPlay = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Block);
	RootComponent = CapsuleComponent;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UZomMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->SetTickGroup(ETickingGroup::TG_DuringPhysics);
	 
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AZomPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AZomPlayer::SetupPlayerInputComponent(UInputComponent* InputComp)
{
	// Movement
	InputComponent->BindAxis(TEXT("Move Forward/Backward"), this, &AZomPlayer::HandleMoveForwardBackwardInput);
	InputComponent->BindAxis(TEXT("Move Right/Left"), this, &AZomPlayer::HandleMoveRightLeftInput);

	// Jump
	InputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AZomPlayer::HandleJumpPressedInput);
	InputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AZomPlayer::HandleJumpReleasedInput);

	// Rotation / Look around
	InputComponent->BindAxis(TEXT("Look Right/Left"), this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis(TEXT("Look Up/Down"), this, &APawn::AddControllerPitchInput);
}

void AZomPlayer::HandleMoveForwardBackwardInput(float Value)
{
	MovementComponent->SetMoveForwardBackwardInput(Value);
}

void AZomPlayer::HandleMoveRightLeftInput(float Value)
{
	MovementComponent->SetMoveRightLeftInput(Value);
}

void AZomPlayer::HandleJumpPressedInput()
{
	MovementComponent->SetJumpInput(true);
}

void AZomPlayer::HandleJumpReleasedInput()
{
	MovementComponent->SetJumpInput(false);
}