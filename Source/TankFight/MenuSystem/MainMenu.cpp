// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"

bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (Host_Button == nullptr) return false;
	Host_Button->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (Join_Menu_Button == nullptr) return false;
	Join_Menu_Button->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (Join_Menu_Back_Button == nullptr) return false;
	Join_Menu_Back_Button->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);
	
	if (Join_Button == nullptr) return false;
	Join_Button->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* menuInterface)
{
	MenuInterface = menuInterface;
}

void UMainMenu::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;
}

void UMainMenu::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController == nullptr) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
}
void UMainMenu::HostServer()
{
	if (MenuInterface == nullptr) return;
	MenuInterface->Host();
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr)
	{
		if (IP_Address_Box == nullptr) return;
		const FString& Address = IP_Address_Box->GetText().ToString();
		MenuInterface->Join(Address);
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (Menu_Switcher == nullptr) return;
	if (Join_Menu == nullptr) return;
	Menu_Switcher->SetActiveWidget(Join_Menu);
}

void UMainMenu::OpenMainMenu()
{
	if (Menu_Switcher == nullptr) return;
	if (Join_Menu == nullptr) return;
	Menu_Switcher->SetActiveWidget(Main_Menu);
}
