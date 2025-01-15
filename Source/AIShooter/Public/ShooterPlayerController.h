// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AShooterPlayerController();
protected:
	virtual void BeginPlay() override;
private:
	TSubclassOf<class UPlayerHUD> PlayerHUDClass = nullptr;
	UPlayerHUD* WBP_HUD = nullptr;
};
