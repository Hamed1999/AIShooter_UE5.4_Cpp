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
	void ResumeGame();
	void OpenLevel();
	UFUNCTION(BlueprintCallable)
		void RestartGame();
	UFUNCTION(BlueprintCallable)
		void ExitGame();
	UFUNCTION(BlueprintPure)
		const FString GetTextMessage();
	void HandleGameOver();
	virtual void GameHasEnded(AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void SetGamePaused(UUserWidget* Widget);

private:
	void CreateIMC();
	void SetIA_Pause();
	void SetIMC();
	void SetPlayerHUDClass();
	void SetPauseMenuClass();
	void SetEndGameClass();
	void SetCheatClass();
	void PauseGame();

	TSubclassOf<class UPlayerHUD> PlayerHUDClass = nullptr;
	UPlayerHUD* WBP_HUD = nullptr;
	TSubclassOf<class UPauseMenu> PauseMenuClass = nullptr;
	UPauseMenu* WBP_PauseMenu = nullptr;
	TSubclassOf<class UUserWidget> EndGameClass = nullptr;
	bool bGameResult = true;
	// Pause Menu Iputs
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		class UInputMappingContext* IMC_Pause;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		class UInputAction* IA_Pause;
};
