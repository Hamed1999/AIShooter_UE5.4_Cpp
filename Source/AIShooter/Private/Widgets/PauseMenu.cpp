// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PauseMenu.h"
#include "Components/Button.h"
#include "ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"

bool UPauseMenu::Initialize()
{
	if (!Super::Initialize()) return false;
	ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::ResumeGame);
	RestartButton->OnClicked.AddDynamic(this, &UPauseMenu::RestartGame);
	ExitButton->OnClicked.AddDynamic(this, &UPauseMenu::ExitGame);
	return true;
}



void UPauseMenu::ResumeGame()
{
	RemoveFromParent();
	if(PlayerController)
		PlayerController->ResumeGame();
}

void UPauseMenu::RestartGame()
{
	if(PlayerController)
		PlayerController->RestartGame();
}

void UPauseMenu::ExitGame()
{
	PlayerController->ConsoleCommand("Exit");
}

void UPauseMenu::SetupController(AShooterPlayerController* NewPlayerController)
{
	PlayerController = NewPlayerController;
}