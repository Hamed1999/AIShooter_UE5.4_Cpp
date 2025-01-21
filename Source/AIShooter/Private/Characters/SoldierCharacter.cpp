// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SoldierCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MovieSceneTracksComponentTypes.h"
#include "ShooterGameMode.h"
#include "Actors/Gun.h"
#include "Components/CapsuleComponent.h"
#include "AIs/EnemyAIController.h"

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

void ASoldierCharacter::CreateSpringArm()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
}

void ASoldierCharacter::CreateCamera()
{
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(SpringArm);
}


ASoldierCharacter::ASoldierCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateSpringArm();
	CreateCamera();
}

void ASoldierCharacter::SpawnGun()
{
	if (!GunClass) return;
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	if (SoldierTeam == ESoldierTeam::PeaceTeam)
		GetMesh()->HideBoneByName(FName("gun"), PBO_None);
	else
	{
		Gun->HideMesh(true);
	}
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("GunSocket"));
	Gun->SetOwner(this);
}

void ASoldierCharacter::SetAIController()
{
	if (!bIsLeader)
	{
		AIControllerClass = AEnemyAIController::StaticClass();
	}
}

void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateMappingContext();
	SpawnGun();
	Health = MaxHealth;
	SetTeamId();
	SetAIController();
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
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &ASoldierCharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Triggered, this, &ASoldierCharacter::Fire);
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
	AddControllerPitchInput(-1*TurnCameraViewValue.Y);
}

AActor* ASoldierCharacter::Shoot()
{
	bIsFiring = true;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]()
		{bIsFiring = false;});
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, 0.01,false);
	return Gun->Shoot();
}

void ASoldierCharacter::Fire()
{
	Shoot();
}

void ASoldierCharacter::CheckGameOver()
{
	AShooterGameMode* ShooterGameMode = GetWorld()->GetAuthGameMode<AShooterGameMode>();
	ShooterGameMode->HandleKills(this);
}

void ASoldierCharacter::HandleDeath()
{
	CheckGameOver();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DetachFromControllerPendingDestroy();
	//GetController()->PawnPendingDestroy(this);
	if (SoldierTeam == ESoldierTeam::PeaceTeam && bIsLeader) 
	{
		GetWorld()->GetFirstPlayerController()->StartSpectatingOnly();
	}
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyTimerDel;
	DestroyTimerDel.BindLambda([&]()
	{
		DestroyIt();
	});
	// GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyTimerDel, 7.0,false);
}

float ASoldierCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                    AActor* DamageCauser)
{
	float ActualDamage =  Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Health -= ActualDamage;
	if (IsDead())
	{
		HandleDeath();
	}
	UE_LOG(LogTemp, Error, TEXT("Health of %s = %f"), *this->GetName(), Health)
	return ActualDamage;
}

bool ASoldierCharacter::IsDead()
{
	return Health <= 0;
}

float ASoldierCharacter::GetHealthPercentage()
{
	return Health / MaxHealth;
}

void ASoldierCharacter::DestroyIt()
{
	if (IsValid(this)){
		if(Gun)
			Gun->Destroy();
		Destroy();
	}
}

void ASoldierCharacter::SetTeamId()
{
	if (SoldierTeam == ESoldierTeam::PeaceTeam)
		TeamId = FGenericTeamId(0);
	else if (SoldierTeam == ESoldierTeam::DevilTeam)
		TeamId = FGenericTeamId(1);
	else
		TeamId = FGenericTeamId(255);
}

FGenericTeamId ASoldierCharacter::GetGenericTeamId() const
{
	return TeamId;
}


void ASoldierCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BindEnhancedInputs(PlayerInputComponent);
}

