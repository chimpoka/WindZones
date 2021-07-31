// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VoronovArtem_AllodsCharacter.generated.h"

class AWindZoneActor;

UCLASS(config=Game)
class AVoronovArtem_AllodsCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AVoronovArtem_AllodsCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	/* Auto generated code ends here. My custom code start below */


public:
	virtual void BeginPlay() override;

	FVector GetActorForwardVector() const { return Super::GetActorForwardVector(); }

	void EnterWindZone(AWindZoneActor* WindZoneActor);
	void ExitWindZone(AWindZoneActor* WindZoneActor);

private:
	void EnableWindMovement();
	void DisableWindMovement();

	void WindModeMove(float AxisValue, FVector InputDirection);
	void WindModeMoveForward(float Value);
	void WindModeMoveRight(float Value);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WindModeMoveSpeed = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float WindModeTurnRate = 2.0f;

private:
	FVector WindWorldDirection;
	TSet<AWindZoneActor*> WindZones;
	bool bWindMovementEnabled;
};

