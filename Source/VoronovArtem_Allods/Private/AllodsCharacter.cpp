// Copyright Epic Games, Inc. All Rights Reserved.

#include "AllodsCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Net/UnrealNetwork.h"

//////////////////////////////////////////////////////////////////////////
// AAllodsCharacter

AAllodsCharacter::AAllodsCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAllodsCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAllodsCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAllodsCharacter::MoveRight);

	PlayerInputComponent->BindAxis("CustomMoveForward", this, &AAllodsCharacter::WindModeMoveForward);
	PlayerInputComponent->BindAxis("CustomMoveRight", this, &AAllodsCharacter::WindModeMoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAllodsCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAllodsCharacter::LookUpAtRate);
}

void AAllodsCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAllodsCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAllodsCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAllodsCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


/* Auto generated code ends here. My custom code start below */


#include "WindZoneActor.h"
#include "Kismet/GameplayStatics.h"

void AAllodsCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAllodsCharacter, bWindMovementEnabled);
}

void AAllodsCharacter::EnterWindZone(AWindZoneActor* WindZoneActor)
{
	WindZones.Add(WindZoneActor);

	if (!bWindMovementEnabled)
	{
		EnableWindMovement();
	}

	UpdateWindWorldDirection();
}

void AAllodsCharacter::ExitWindZone(AWindZoneActor* WindZoneActor)
{
	WindZones.Remove(WindZoneActor);

	if (WindZones.Num() == 0)
	{
		DisableWindMovement();
	}

	UpdateWindWorldDirection();
}

void AAllodsCharacter::UpdateWindWorldDirection()
{
	WindWorldDirection = FVector::ZeroVector;
	for (const auto& windZone : WindZones)
	{
		WindWorldDirection += windZone->WindDirection;
	}
}

void AAllodsCharacter::EnableWindMovement()
{
	GetCharacterMovement()->DisableMovement();
	bWindMovementEnabled = true;

	// It works automatically on clients, but we must call it manually for server here
	OnRep_bWindMovementEnabled();
}

void AAllodsCharacter::DisableWindMovement()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	bWindMovementEnabled = false;

	// It works automatically on clients, but we must call it manually for server here
	OnRep_bWindMovementEnabled();
}

void AAllodsCharacter::WindModeMoveForward(float Value)
{
	if (!IsValidForServer_AddCharacterInputInWindZone(Value))
		return;

	Server_AddCharacterInputInWindZone(Value, EAxis::X);
}

void AAllodsCharacter::WindModeMoveRight(float Value)
{
	if (!IsValidForServer_AddCharacterInputInWindZone(Value))
		return;

	Server_AddCharacterInputInWindZone(Value, EAxis::Y);
}

bool AAllodsCharacter::IsValidForServer_AddCharacterInputInWindZone(float AxisValue)
{
	if (!IsLocallyControlled())
		return false;

	if (!bWindMovementEnabled)
		return false;

	if (FMath::IsNearlyEqual(AxisValue, 0.0f))
		return false;

	return true;
}

void AAllodsCharacter::Server_AddCharacterInputInWindZone_Implementation(float AxisValue, EAxis::Type AxisDirection)
{
	WindModeMove(AxisValue, AxisDirection);
}

bool AAllodsCharacter::Server_AddCharacterInputInWindZone_Validate(float AxisValue, EAxis::Type AxisDirection)
{
	if (AxisValue < -1.01f || AxisValue > 1.01f)
		return false;

	if (AxisDirection == EAxis::Z)
		return false;

	return true;
}

void AAllodsCharacter::WindModeMove(float AxisValue, EAxis::Type AxisDirection)
{
	if (Controller == nullptr)
		return;

	// Find out which way is forward
	const FRotator rotation = Controller->GetControlRotation();
	const FRotator yawRotation(0, rotation.Yaw, 0);

	// Get input vector
	const FVector inputDirection = FRotationMatrix(yawRotation).GetUnitAxis(AxisDirection);

	FVector movementForwardVector = inputDirection * AxisValue * WindModeMoveSpeed;
	FVector projectionOnWindDirection = movementForwardVector.ProjectOnTo(WindWorldDirection);
	// Exclude WindWorldDirection projection from inputDirection. Don't want to speed up/slow down character inside wind zone,
	// want to keep constant speed towards WindWorldDirection
	FVector onlySideMoveDirection = movementForwardVector - projectionOnWindDirection;
	AddActorWorldOffset(onlySideMoveDirection);

	FQuat from = GetActorQuat();
	FQuat to = movementForwardVector.ToOrientationQuat();
	SetActorRotation(FMath::QInterpTo(from, to, UGameplayStatics::GetWorldDeltaSeconds(this), WindModeTurnRate));
}