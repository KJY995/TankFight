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

protected:

	virtual void BeginPlay() override;

private:

	class ATankPawn* Tank;
};



