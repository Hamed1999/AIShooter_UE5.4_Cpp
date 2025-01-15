// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Character.h"
#include "SoldierCharacter.generated.h"

UENUM(BlueprintType)
enum class ESoldierTeam : uint8
{
	PeaceTeam UMETA(DisplayName = "Peace Team"),
	DevilTeam UMETA(DisplayName = "Devil Team")
};

UCLASS()
class AISHOOTER_API ASoldierCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ASoldierCharacter();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite)
		bool bIsFiring = false;
	virtual FGenericTeamId GetGenericTeamId() const override;
	AActor* Shoot();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool bIsLeader = false;
	UFUNCTION(BlueprintPure)
		bool IsDead();
	float GetHealthPercentage();
private:
	/**
	 * Methods
	 */
	void CreateMappingContext();
	void CreateSpringArm();
	void CreateCamera();
	void SpawnGun();
	void SetAIController();
	void BindEnhancedInputs(UInputComponent* PlayerInputComponent);
	void MoveForward(const struct FInputActionValue& InputValue);
	void MoveRight(const  FInputActionValue& InputValue);
	void TurnCameraView(const  FInputActionValue& InputValue);
	void Fire();
	void HandleDeath();
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void SetTeamId();
	/**
	 * Global Properties
	 */
	UPROPERTY(EditAnywhere, Category= "Gun", BlueprintReadWrite, Meta = (AllowPrivateAccess))
	TSubclassOf<class AGun> GunClass;
	AGun* Gun = nullptr;
	// Copmponents
	UPROPERTY(EditAnywhere, Category= "Components", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		class USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, Category= "Components", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		class UCameraComponent* Camera;
	// Default Iputs
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		class UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		class UInputAction* IA_MovingForward;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		 UInputAction* IA_MovingRight;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		 UInputAction* IA_CameraView;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UInputAction* IA_Fire;
	// Properties
	UPROPERTY(EditAnywhere, Category= "Health", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0.0"))
		float MaxHealth = 100;
	float Health;
	UPROPERTY(EditAnywhere, Category= "Team", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		ESoldierTeam SoldierTeam = ESoldierTeam::PeaceTeam;
	UPROPERTY(EditAnywhere, Category= "Team", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		FGenericTeamId TeamId = FGenericTeamId(0);
};
