// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankFightGameMode.generated.h"

UCLASS(minimalapi)
class ATankFightGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATankFightGameMode();

	void ActorDied(AActor* deadActor);
	
	void TankFightGameOver(bool isWin);

	void GameOverEvent(bool winner);
	
	bool IsWinner = true;

protected:

	virtual void BeginPlay() override;

private:

	class ATankPawn* Tank;
};



