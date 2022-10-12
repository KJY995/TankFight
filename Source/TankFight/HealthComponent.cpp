// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "TankFightGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Components/InputComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::DamageTaken);
	TankFightGameMode = Cast<ATankFightGameMode>(UGameplayStatics::GetGameMode(this));
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::DamageTaken(AActor *DamagedActor, float Damage, const UDamageType *DamageType, class AController *Instigator, AActor *DamageCauser)
{
	if(Damage>=0.f)
	{
		Health = Health - Damage;
		UE_LOG(LogTemp, Warning, TEXT("Health: %f"), Health);

		if (Health <= 0.f)
		{
			if(TankFightGameMode == nullptr) 
			{
				UE_LOG(LogTemp, Warning, TEXT("TankFightGameMode Fail"));
				return;
			}
			UE_LOG(LogTemp, Warning, TEXT("TankFightGameMode Called"));
			TankFightGameMode->ActorDied(DamagedActor);
		}
		
	}
}