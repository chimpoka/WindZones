// Copyright Epic Games, Inc. All Rights Reserved.

#include "LavaActor.h"
#include "AllodsGameMode.h"
#include "AllodsCharacter.h"

#include "Kismet/GameplayStatics.h"

ALavaActor::ALavaActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SceneRoot->SetMobility(EComponentMobility::Static);
	SetRootComponent(SceneRoot);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMeshComponent->SetupAttachment(GetRootComponent());
}

void ALavaActor::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->OnComponentHit.AddDynamic(this, &ALavaActor::OnComponentHit);
}

void ALavaActor::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!HasAuthority())
		return;

	auto gameMode = Cast<AAllodsGameMode>(UGameplayStatics::GetGameMode(this));
	auto character = Cast<AAllodsCharacter>(OtherActor);

	gameMode->RespawnCharacter(character);
}
