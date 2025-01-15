 // Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "Widgets/PlayerHUD.h"
#include "UObject/ConstructorHelpers.h"

 AShooterPlayerController::AShooterPlayerController()
 {
  ConstructorHelpers::FClassFinder<UPlayerHUD> PlayerHUDRef(TEXT("/Game/Widgets/WBP_HUD"));
  if (PlayerHUDRef.Succeeded())
    PlayerHUDClass = PlayerHUDRef.Class.Get();
 }

 void AShooterPlayerController::BeginPlay()
 {
   Super::BeginPlay();
    if (PlayerHUDClass)
    {
      WBP_HUD = CreateWidget<UPlayerHUD>(this, PlayerHUDClass);
    	WBP_HUD->SetOwnerCharacter(GetCharacter());
    }
    if (WBP_HUD)
		       WBP_HUD->AddToViewport();
 }