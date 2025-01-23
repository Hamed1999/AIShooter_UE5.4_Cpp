 // Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCheatManager.h"
#include "Characters/SoldierCharacter.h"
#include "Kismet/GameplayStatics.h"

 void UShooterCheatManager::HackGun(int Ammo)
{
 	SetSoldier();
	 if(Soldier)
	{
		Soldier->SetAmmo(Ammo);
	}
}

 void UShooterCheatManager::HackHealth(float NewHealth)
 {
 	SetSoldier();
   if(Soldier)
     if(auto PlayerSoldier = Cast<ASoldierCharacter>(Soldier))
       PlayerSoldier->SetHealth(NewHealth);
 }

 void UShooterCheatManager::SetSoldier()
 {
 	APawn* PlayerCharacter = UGameplayStatics::GetPlayerPawn(this, 0);
 	Soldier = Cast<ASoldierCharacter>(PlayerCharacter);
 }
