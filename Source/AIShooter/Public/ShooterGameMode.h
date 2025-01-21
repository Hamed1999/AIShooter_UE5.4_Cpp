// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShooterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_API AShooterGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	AShooterGameMode();
	void HandleKills(APawn* KilledPawn);
protected:
	void BeginPlay() override;
private:
	class ASoldierCharacter* FirstPlayerCharacter = nullptr;
};
