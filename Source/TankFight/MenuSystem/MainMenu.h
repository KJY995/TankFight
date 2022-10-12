// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class TANKFIGHT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);

	void Setup();
	void Teardown();
	
protected:
	virtual bool Initialize();

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Host_Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join_Menu_Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join_Button;

	UPROPERTY(meta = (BindWidget))
	class UButton* Join_Menu_Back_Button;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* Menu_Switcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Main_Menu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* Join_Menu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IP_Address_Box;
	
	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();


	IMenuInterface* MenuInterface;
};
