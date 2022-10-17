// Copyright Epic Games, Inc. All Rights Reserved.

#include "TankFightGameMode.h"
#include "TankFightCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "TankPawn.h"

ATankFightGameMode::ATankFightGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATankFightGameMode::ActorDied(AActor *DeadActor)
{
    
	if(DeadActor == nullptr) 
    {
		UE_LOG(LogTemp, Warning, TEXT("Null DeadActor"));
        return;
    }
    UE_LOG(LogTemp, Warning, TEXT("%s"), *DeadActor->GetName());
    if (DeadActor == Tank)
    {
        Tank->HandleDestruction();
    }
    else if(ATankPawn * DeadOpponent = Cast<ATankPawn>(DeadActor))
    {
        DeadOpponent->HandleDestruction();
    }
}
void ATankFightGameMode::TankFightGameOver(bool isWin)
{
    if (isWin)
    {
        UE_LOG(LogTemp, Warning, TEXT("Win!"));
        GameOverEvent(true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Lose!"));
        GameOverEvent(false);
    }
}
void ATankFightGameMode::GameOverEvent(bool winner)
{
}

void ATankFightGameMode::BeginPlay()
{    
    
	Super::BeginPlay();

    Tank = Cast<ATankPawn>(UGameplayStatics::GetPlayerPawn(this, 0)); 
}
