// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CannonShell.h"
#include "HealthComponent.h"
#include "TankPawn.generated.h"

UCLASS()
class TANKFIGHT_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void HandleDestruction();
	APlayerController* GetTankPlayerController() const;


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void HeadRotation(FVector Target);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_HeadRotation(FRotator headRotation);

private:
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Speed = 750.f;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 90.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Body;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Head;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Cannon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* FirePoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Combat")
	TSubclassOf<ACannonShell> CannonShellClass;

	APlayerController* PlayerControllerRef;
	void Move(float Value);
	void Turn(float Value);

	void FireCannon();
	void UpdateLocation(float DeltaTime);
	void UpdateRotation(float DeltaTime);

	UPROPERTY(Replicated)
	float Throttle;
	UPROPERTY(Replicated)
	float Steering;

	UFUNCTION()
	void OnRep_ReplicatedTransform();
	
	UFUNCTION()
	void OnRep_ReplicatedHeadRotation();

	UPROPERTY(ReplicatedUsing=OnRep_ReplicatedHeadRotation)
	FRotator ReplicatedHeadRotation;

	UPROPERTY(ReplicatedUsing=OnRep_ReplicatedTransform)
	FTransform ReplicatedTransform;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Move(float Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Turn(float Value);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_FireCannon();
};