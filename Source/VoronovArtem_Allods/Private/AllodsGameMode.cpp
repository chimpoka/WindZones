// Copyright Epic Games, Inc. All Rights Reserved.

#include "AllodsGameMode.h"
#include "AllodsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAllodsGameMode::AAllodsGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
}

void AAllodsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	PlayerControllerList.Add(NewPlayer);
}

void AAllodsGameMode::RespawnCharacter(AAllodsCharacter* Character)
{
	for (auto playerController : PlayerControllerList)
	{
		auto characterFromList = Cast<AAllodsCharacter>(playerController->GetPawn());
		
		if (Character == characterFromList)
		{
			Character->Destroy();
			RestartPlayer(playerController);
			return;
		}
	}
}
