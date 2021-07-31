// Copyright Epic Games, Inc. All Rights Reserved.

#include "VoronovArtem_AllodsGameMode.h"
#include "VoronovArtem_AllodsCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVoronovArtem_AllodsGameMode::AVoronovArtem_AllodsGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/ThirdPersonCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
}
