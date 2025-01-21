// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual bool Initialize() override;
	void SetupController(class AShooterPlayerController* NewPlayerController);
private:
	UFUNCTION(BlueprintCallable, Category = "Buttons")
		void ResumeGame();
	UFUNCTION(BlueprintCallable, Category = "Buttons")
		void RestartGame();
	UFUNCTION(BlueprintCallable, Category = "Buttons")
		void ExitGame();
	
	UPROPERTY(Meta = (BindWidget))
		class UButton* ResumeButton;
	UPROPERTY(Meta = (BindWidget))
		UButton* RestartButton;
	UPROPERTY(Meta = (BindWidget))
		UButton* ExitButton;

	AShooterPlayerController* PlayerController = nullptr;
};


