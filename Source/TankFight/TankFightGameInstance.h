// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuSystem/MenuInterface.h"
#include "MenuSystem/MainMenu.h"
#include "TankFightGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TANKFIGHT_API UTankFightGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UTankFightGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(const FString& Address);

private:
	TSubclassOf<class UUserWidget> MenuClass;
	
	UMainMenu* Menu;

};
