// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

	public:
	void SetOwnerCharacter(ACharacter* OwnerCharacter);
private:
	UFUNCTION(BlueprintPure, Category="Health")
		float GetPlayerHealth();
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess))
		class ASoldierCharacter* PlayerCharacter = nullptr;
};
