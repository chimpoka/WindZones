// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AllodsGameMode.generated.h"

class APlayerController;
class AAllodsCharacter;

UCLASS(minimalapi)
class AAllodsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAllodsGameMode() = default;

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	void RespawnCharacter(AAllodsCharacter* Character);

private:
	TArray<APlayerController*> PlayerControllerList;
};



