// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPawn.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "Components/SceneComponent.h"
#include "TankFightGameMode.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	RootComponent = BoxComp;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(RootComponent);

	Head = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(Body);

	Cannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon"));
	Cannon->SetupAttachment(Head);

	FirePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	FirePoint->SetupAttachment(Cannon);

    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerControllerRef = Cast<APlayerController>(GetController());
    TankFightGameMode = Cast<ATankFightGameMode>(UGameplayStatics::GetGameMode(this));
}

void ATankPawn::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ATankPawn, CannonShellClass);
    DOREPLIFETIME(ATankPawn, ReplicatedTransform);
    DOREPLIFETIME(ATankPawn, ReplicatedHeadRotation);
    DOREPLIFETIME(ATankPawn, Throttle);
    DOREPLIFETIME(ATankPawn, Steering);
    DOREPLIFETIME(ATankPawn, IsDead);
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    UpdateRotation(DeltaTime);
    UpdateLocation(DeltaTime);
    if(HasAuthority())
    {
        ReplicatedTransform = GetActorTransform();
        ReplicatedHeadRotation = Head->GetComponentRotation();
    }

    if(HasAuthority()) DrawDebugString(GetWorld(), FVector(0, 0, 100), "Host", this, FColor::White, DeltaTime);
    else DrawDebugString(GetWorld(), FVector(0, 0, 100), "Guest", this, FColor::White, DeltaTime);
    if (PlayerControllerRef)
    {
        FHitResult HitResult;
        PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 1.f, 4, FColor::Red, false, -1.f);

        HeadRotation(HitResult.ImpactPoint);
	}
    if (IsDead)
    {
        Destruction();
    }
}

APlayerController* ATankPawn::GetTankPlayerController() const
{
    return PlayerControllerRef; 
}

void ATankPawn::OnRep_ReplicatedTransform()
{
    SetActorTransform(ReplicatedTransform);
}

void ATankPawn::OnRep_ReplicatedHeadRotation()
{
    Head->SetWorldRotation(ReplicatedHeadRotation);
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    
    PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &ATankPawn::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATankPawn::Turn);
    //PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATankPawn::Server_FireCannon);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATankPawn::Server_FireCannon);
}

void ATankPawn::HeadRotation(FVector Target)
{
	FVector HeadDirection = Target - Head->GetComponentLocation();
	FRotator CannonRotation = FRotator(0.f, HeadDirection.Rotation().Yaw, 0.f);
	Head->SetWorldRotation(CannonRotation);
    Server_HeadRotation(CannonRotation);
}
void ATankPawn::Server_HeadRotation_Implementation(FRotator headRotation)
{
	Head->SetWorldRotation(headRotation);
}
bool ATankPawn::Server_HeadRotation_Validate(FRotator headRotation)
{
    return true;
}


void ATankPawn::UpdateLocation(float DeltaTime)
{
    FVector DeltaLocation = FVector::ZeroVector;
    DeltaLocation.X = Throttle * Speed * DeltaTime;
    AddActorLocalOffset(DeltaLocation, true);
}
void ATankPawn::UpdateRotation(float DeltaTime)
{
    FRotator DeltaRotation = FRotator::ZeroRotator;
    DeltaRotation.Yaw = Steering * TurnRate * DeltaTime;
    AddActorLocalRotation(DeltaRotation, true);
}

void ATankPawn::Move(float Value)
{
    Throttle = Value;
    Server_Move(Value);
}
void ATankPawn::Server_Move_Implementation(float Value)
{
    Throttle=Value;
}
bool ATankPawn::Server_Move_Validate(float Value)
{
    return FMath::Abs(Value) <= 1;
}

void ATankPawn::Turn(float Value)
{
    Steering = Value;
    Server_Turn(Value);
}
void ATankPawn::Server_Turn_Implementation(float Value)
{
    Steering=Value;
}
bool ATankPawn::Server_Turn_Validate(float Value)
{
    return FMath::Abs(Value) <= 1;
}

void ATankPawn::FireCannon()
{
	FVector FirePointLocation = FirePoint->GetComponentLocation();
	FRotator FirePointRotation = FirePoint->GetComponentRotation();
	
	GetWorld()->SpawnActor<ACannonShell>(CannonShellClass, FirePointLocation, FirePointRotation);
}

void ATankPawn::Server_FireCannon_Implementation()
{
	FVector FirePointLocation = FirePoint->GetComponentLocation();
	FRotator FirePointRotation = FirePoint->GetComponentRotation();
	
	auto cannonShell = GetWorld()->SpawnActor<ACannonShell>(CannonShellClass, FirePointLocation, FirePointRotation);
    cannonShell->SetOwner(this);
}

bool ATankPawn::Server_FireCannon_Validate()
{
    return true;
}


void ATankPawn::HandleDestruction()
{
    IsDead = true;
}

void ATankPawn::Destruction()
{
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    if (this->GetTankPlayerController())
    {

        UE_LOG(LogTemp, Warning, TEXT("Disable Input Called"));
        this->DisableInput(this->GetTankPlayerController());
        this->GetTankPlayerController()->bShowMouseCursor = false;
        TankFightGameMode->TankFightGameOver(false);
    }
    else
    {
        TankFightGameMode->TankFightGameOver(true);
    }
}

/*
//----------------------------------------------Move
void ATankPawn::Server_Move_Implementation(float Value)
{
    Throttle=Value;
}
bool ATankPawn::Server_Move_Validate(float Value)
{
    return FMath::Abs(Value) <= 1;
}

//----------------------------------------------Turn
void ATankPawn::Server_Turn_Implementation(float Value)
{
    Steering=Value;
}
bool ATankPawn::Server_Turn_Validate(float Value)
{
    return FMath::Abs(Value) <= 1;
}
*/
/*
void ATankPawn::FireCannon()
{
	FVector FirePointLocation = FirePoint->GetComponentLocation();
	FRotator FirePointRotation = FirePoint->GetComponentRotation();
	
	GetWorld()->SpawnActor<ACannonShell>(CannonShellClass, FirePointLocation, FirePointRotation);

    Server_FireCannon();
}

void ATankPawn::Server_FireCannon_Implementation()
{
	FVector FirePointLocation = FirePoint->GetComponentLocation();
	FRotator FirePointRotation = FirePoint->GetComponentRotation();
	
	GetWorld()->SpawnActor<ACannonShell>(CannonShellClass, FirePointLocation, FirePointRotation);
}

bool ATankPawn::Server_FireCannon_Validate()
{
    return true;
}
*/