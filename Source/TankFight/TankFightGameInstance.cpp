// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFightGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

UTankFightGameInstance::UTankFightGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/TankFight/Menu/WBP_MainMenu"));
	if (MenuBPClass.Class == nullptr) return;

	MenuClass = MenuBPClass.Class;
}

void UTankFightGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());
}

void UTankFightGameInstance::LoadMenu()
{
	if (MenuClass == nullptr) return;
	
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (Menu == nullptr) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void UTankFightGameInstance::Host()
{
	if(Menu == nullptr) return;
	Menu->Teardown();

    UWorld* World = GetWorld();
	if (World == nullptr) return;

	World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
}

void UTankFightGameInstance::Join(const FString& Address)
{
	if (Menu == nullptr) return;
	
	Menu->Teardown();

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (PlayerController == nullptr) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}