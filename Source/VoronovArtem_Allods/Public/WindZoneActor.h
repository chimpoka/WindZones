// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindZoneActor.generated.h"

class UArrowComponent;
class UStaticMeshComponent;
class AAllodsCharacter;

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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Allods)
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

	UPROPERTY(EditAnywhere, Category = Allods)
	bool bForwardDirection;

	UPROPERTY()
	AAllodsCharacter* AffectedCharacter;
};
