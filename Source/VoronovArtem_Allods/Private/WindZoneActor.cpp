// Fill out your copyright notice in the Description page of Project Settings.


#include "WindZoneActor.h"
#include <Components/ArrowComponent.h>
#include <Components/StaticMeshComponent.h>

#include "AllodsCharacter.h"

AWindZoneActor::AWindZoneActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SceneRoot->SetMobility(EComponentMobility::Static);
	SetRootComponent(SceneRoot);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("Arrow");
	ArrowComponent->SetupAttachment(GetRootComponent());

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	bReplicates = true;
}

void AWindZoneActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorTickEnabled(false);

	// I think it's a good idea to check overlaps only on server side
	if (!HasAuthority())
		return;

	StaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AWindZoneActor::OnComponentBeginOverlap);
	StaticMeshComponent->OnComponentEndOverlap.AddDynamic(this, &AWindZoneActor::OnComponentEndOverlap);
}

void AWindZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(AffectedCharacter))
		return;

	AffectedCharacter->AddActorWorldOffset(WindDirection * WindSpeed);
}

void AWindZoneActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetForwardDirection(bForwardDirection);
}

void AWindZoneActor::SetForwardDirection(bool ForwardDirection)
{
	WindDirection = ForwardDirection ? GetActorForwardVector() : -GetActorForwardVector();
	ArrowComponent->SetWorldRotation(WindDirection.ToOrientationQuat());
}

void AWindZoneActor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto tempCharacter = Cast<AAllodsCharacter>(OtherActor);

	if (!IsValid(tempCharacter))
		return;

	SetActorTickEnabled(true);
	AffectedCharacter = tempCharacter;
	AffectedCharacter->EnterWindZone(this);
}

void AWindZoneActor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(AffectedCharacter))
		return;

	SetActorTickEnabled(false);
	AffectedCharacter->ExitWindZone(this);
	AffectedCharacter = nullptr;
}
