// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PlayerHUD.h"

#include "Characters/SoldierCharacter.h"

void UPlayerHUD::SetOwnerCharacter(ACharacter* OwnerCharacter)
{
	PlayerCharacter = Cast<ASoldierCharacter>(OwnerCharacter);
}

float UPlayerHUD::GetPlayerHealth()
{
	if(PlayerCharacter)
		return PlayerCharacter->GetHealthPercentage();
	return 100;
}
