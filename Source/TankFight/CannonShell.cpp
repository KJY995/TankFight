// Fill out your copyright notice in the Description page of Project Settings.
#include "CannonShell.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACannonShell::ACannonShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TankShell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shell"));
	RootComponent = TankShell;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Shell Movement Comp"));
	ProjectileMovementComponent->MaxSpeed = 2000.f;
	ProjectileMovementComponent->InitialSpeed = 2000.f;

	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
}

// Called when the game starts or when spawned
void ACannonShell::BeginPlay()
{
	Super::BeginPlay();
	TankShell->OnComponentHit.AddDynamic(this, &ACannonShell::OnHit);
}

void ACannonShell::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ACannonShell, ReplicatedTransform);
    //DOREPLIFETIME(ATankPawn, ReplicatedHeadRotation);
}


// Called every frame
void ACannonShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACannonShell::OnRep_ReplicatedTransform()
{
    SetActorTransform(ReplicatedTransform);
}

void ACannonShell::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit)
{
	if(GetOwner() == nullptr) return;

	auto myOwner = GetOwner();
	auto myInstigator = myOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if(OtherActor != nullptr && OtherActor != this && OtherActor != myOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, myInstigator, this, DamageTypeClass);
		Destroy();
	}
}