// Copyright Epic Games, Inc. All Rights Reserved.

#include "AllodsGameMode.h"
#include "AllodsCharacter.h"
#include "UObject/ConstructorHelpers.h"

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
