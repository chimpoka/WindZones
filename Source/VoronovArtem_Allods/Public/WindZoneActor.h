// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindZoneActor.generated.h"

class UArrowComponent;
class UStaticMeshComponent;
class AVoronovArtem_AllodsCharacter;

UCLASS()
class VORONOVARTEM_ALLODS_API AWindZoneActor : public AActor
{
	GENERATED_BODY()

public:	
	AWindZoneActor();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	void SetForwardDirection(bool ForwardDirection);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY()
		float WindSpeed = 2.0f;

	UPROPERTY()
	FVector WindDirection;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComponent;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	bool bForwardDirection;

	UPROPERTY()
	AVoronovArtem_AllodsCharacter* AffectedCharacter;
};
