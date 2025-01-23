// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "ShooterCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_API UShooterCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
private:
	UFUNCTION(Exec)
		void HackGun(int Ammo = 999);
	UFUNCTION(Exec)
		void HackHealth(float NewHealth = 999);
	void SetSoldier();
	class ASoldierCharacter* Soldier = nullptr;
};
