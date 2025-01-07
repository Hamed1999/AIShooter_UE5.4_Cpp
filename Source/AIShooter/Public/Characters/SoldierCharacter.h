// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SoldierCharacter.generated.h"

UCLASS()
class AISHOOTER_API ASoldierCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASoldierCharacter();
	void SpawnGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, Category= "Default Inputs", BlueprintReadWrite)
		bool bIsFiring = false;
private:
	/**
	 * Methods
	 */
	void CreateMappingContext();
	void CreateSpringArm();
	void CreateCamera();
	void BindEnhancedInputs(UInputComponent* PlayerInputComponent);
	void MoveForward(const struct FInputActionValue& InputValue);
	void MoveRight(const  FInputActionValue& InputValue);
	void TurnCameraView(const  FInputActionValue& InputValue);
	void Fire();
	
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
};
