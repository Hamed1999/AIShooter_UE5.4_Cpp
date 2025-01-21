 // Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Widgets/PlayerHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/PauseMenu.h"
#include "Characters/SoldierCharacter.h"



 void AShooterPlayerController::CreateIMC()
 {
	 if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	 {
		 Subsystem->AddMappingContext(IMC_Pause, 0);
	 }
 }

 void AShooterPlayerController::SetIA_Pause()
 {
	 ConstructorHelpers::FObjectFinder<UInputAction> IA_PauseRef(TEXT("/Game/Inputs/IA_Pause"));
	 if (IA_PauseRef.Succeeded())
		 IA_Pause = IA_PauseRef.Object.Get();
 }

 void AShooterPlayerController::SetIMC()
 {
	 ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Ref(TEXT("/Game/Inputs/IMC_Pause"));
	 if (IMC_Ref.Succeeded())
		 IMC_Pause = IMC_Ref.Object.Get();
 }

 void AShooterPlayerController::SetPlayerHUDClass()
 {
	 ConstructorHelpers::FClassFinder<UPlayerHUD> PlayerHUDRef(TEXT("/Game/Widgets/WBP_HUD"));
	 if (PlayerHUDRef.Succeeded())
		 PlayerHUDClass = PlayerHUDRef.Class.Get();
 }

 void AShooterPlayerController::SetPauseMenuClass()
 {
	 ConstructorHelpers::FClassFinder<UPauseMenu> PauseMenuRef(TEXT("/Game/Widgets/WBP_PauseMenu"));
	 if (PauseMenuRef.Succeeded())
		 PauseMenuClass = PauseMenuRef.Class.Get();
 }

 void AShooterPlayerController::SetEndGameClass()
 {
	 ConstructorHelpers::FClassFinder<UUserWidget> EndGameRef(TEXT("/Game/Widgets/WBP_EndGame"));
	 if (EndGameRef.Succeeded())
		 EndGameClass = EndGameRef.Class.Get();
 }

 AShooterPlayerController::AShooterPlayerController()
 {
 	SetIMC();
	SetIA_Pause();
 	SetPlayerHUDClass();
 	SetPauseMenuClass();
 	SetEndGameClass();
 }

 void AShooterPlayerController::ResumeGame()
 {
 	EnableInput(this);
 	UGameplayStatics::SetGamePaused(this, false);
 	SetShowMouseCursor(false);
 	SetInputMode(FInputModeGameOnly());
 }

 void AShooterPlayerController::RestartGame()
 {
 	TSubclassOf<ASoldierCharacter> SoldierClass = ASoldierCharacter::StaticClass();
 	TArray<AActor*> AllSoldiers;
 	UGameplayStatics::GetAllActorsOfClass(this, SoldierClass, AllSoldiers);
 	ResumeGame();
 	UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestLevel"));
 	
 }

 void AShooterPlayerController::ExitGame()
 {
 	ConsoleCommand("Quit");
 }

 const FString AShooterPlayerController::GetTextMessage()
 {
 	const FString WinMessage = FString("WoW.\n You Win : ) ...");
 	const FString LoseMessage = FString("Oops.\n Such A Looser ...");
 	if (bGameResult) return WinMessage;
 	return LoseMessage;
 }

 void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
 {
 	Super::GameHasEnded(EndGameFocus, bIsWinner);
 	bGameResult = bIsWinner;
 	UUserWidget* WBP_EndGame = CreateWidget<UUserWidget>(this, EndGameClass);
 	WBP_EndGame->AddToViewport();
 	FTimerHandle TimerHandle;
 	GetWorldTimerManager().SetTimer(TimerHandle,this, &AShooterPlayerController::SetGamePaused, 2.0, false);
 }


 void AShooterPlayerController::BeginPlay()
 {
   Super::BeginPlay();
 	CreateIMC();
    if (PlayerHUDClass)
    {
      WBP_HUD = CreateWidget<UPlayerHUD>(this, PlayerHUDClass);
    	WBP_HUD->SetOwnerCharacter(GetCharacter());
    }
    if (WBP_HUD)
		       WBP_HUD->AddToViewport();
 }

 void AShooterPlayerController::SetupInputComponent()
 {
	 Super::SetupInputComponent();

 	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
 		EnhancedInputComponent->BindAction(IA_Pause, ETriggerEvent::Triggered, this, &AShooterPlayerController::PauseGame);
 	 
 }

 void AShooterPlayerController::SetGamePaused()
 {
	 DisableInput(this);
	 UGameplayStatics::SetGamePaused(this, true);
	 SetShowMouseCursor(true);
	 SetInputMode(FInputModeUIOnly());
 }

 void AShooterPlayerController::PauseGame()
 {
 	if (PauseMenuClass)
 	{
 		WBP_PauseMenu = CreateWidget<UPauseMenu>(this, PauseMenuClass);
 	}
 	if (WBP_PauseMenu)
 	{
 		WBP_PauseMenu->AddToViewport();
 		WBP_PauseMenu->SetupController(this);
 		SetGamePaused();
 	}
 }

