// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SoldierCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ASoldierCharacter::CreateMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Sets default values
ASoldierCharacter::ASoldierCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoldierCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASoldierCharacter::BindEnhancedInputs(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MovingForward, ETriggerEvent::Triggered, this, &ASoldierCharacter::MoveForward);
		EnhancedInputComponent->BindAction(IA_MovingRight, ETriggerEvent::Triggered, this, &ASoldierCharacter::MoveRight);
		EnhancedInputComponent->BindAction(IA_CameraView, ETriggerEvent::Triggered, this, &ASoldierCharacter::TurnCameraView);
	}
}

void ASoldierCharacter::MoveForward(const FInputActionValue& InputValue)
{
	const float ForwardValue =  InputValue.Get<float>();
	AddMovementInput(GetActorForwardVector() * ForwardValue);
}

void ASoldierCharacter::MoveRight(const FInputActionValue& InputValue)
{
	const float RightdValue =  InputValue.Get<float>();
	AddMovementInput(GetActorRightVector() * RightdValue);
}

void ASoldierCharacter::TurnCameraView(const FInputActionValue& InputValue)
{
	const FVector2D TurnCameraViewValue = InputValue.Get<FVector2D>();
	AddControllerYawInput(TurnCameraViewValue.X);
	AddControllerPitchInput(TurnCameraViewValue.Y);
}

void ASoldierCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindEnhancedInputs(PlayerInputComponent);
}

