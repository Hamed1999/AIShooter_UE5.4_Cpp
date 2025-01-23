// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SoldierCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShooterGameMode.h"
#include "Actors/Gun.h"
#include "Components/CapsuleComponent.h"
#include "AIs/EnemyAIController.h"
#include "Blueprint/UserWidget.h"

void ASoldierCharacter::SetAmmo(int Ammo)
{
	if (Ammo < 0) return;
	if(GunIsSafe()) Guns[ActiveGunIndex]->SetAmmo(Ammo);
}

void ASoldierCharacter::SetHealth(float NewHealth)
{
	Health = NewHealth;
	MaxHealth = NewHealth;
}

AGun* ASoldierCharacter::GetActiveGun()
{
	if(GunIsSafe())
		return Guns[ActiveGunIndex];
	return nullptr;
}
int ASoldierCharacter::GetAmmo()
{
	int Ammo = 0;
	if(GunIsSafe()) Ammo = GetActiveGun()->CurrentAmmo;
	return Ammo;
}

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


void ASoldierCharacter::SetSniperViewClass()
{
	ConstructorHelpers::FClassFinder<UUserWidget> SniperViewRef(TEXT("/Game/Widgets/WBP_SniperView"));
	if (SniperViewRef.Succeeded())
		SniperViewClass = SniperViewRef.Class.Get();
}

ASoldierCharacter::ASoldierCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CreateSpringArm();
	CreateCamera();
	SetSniperViewClass();
}

void ASoldierCharacter::SpawnGun()
{
	Guns.SetNum(GunClasses.Num());
	for (int i = 0; i < GunClasses.Num(); i++)
	{
		
		if (auto GunClass = GunClasses[i])
		{
			Guns[i] = GetWorld()->SpawnActor<AGun>(GunClass);
			if (SoldierTeam == ESoldierTeam::PeaceTeam)
				GetMesh()->HideBoneByName(FName("gun"), PBO_None);
			else
			{
				Guns[i]->HideMesh(true);
			}
			Guns[i]->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("GunSocket"));
			Guns[i]->SetOwner(this);
		}
	}
	ManageGuns();
}

void ASoldierCharacter::SetAIController()
{
	if (!bIsLeader)
	{
		AIControllerClass = AEnemyAIController::StaticClass();
	}
}

void ASoldierCharacter::SetTriggerIntervals()
{
	Cast<UInputTriggerPulse>(IA_Fire->Triggers[0])->Interval = Guns[ActiveGunIndex]->TriggerIntervals;
}

void ASoldierCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateMappingContext();
	SpawnGun();
	Health = MaxHealth;
	SetTeamId();
	SetAIController();
	SetTriggerIntervals();
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
		EnhancedInputComponent->BindAction(IA_ChangeGun, ETriggerEvent::Triggered, this, &ASoldierCharacter::ChangeGun);
		EnhancedInputComponent->BindAction(IA_Zoom, ETriggerEvent::Triggered, this, &ASoldierCharacter::Zoom);
		EnhancedInputComponent->BindAction(IA_Reload, ETriggerEvent::Triggered, this, &ASoldierCharacter::Reload);
	}
}

void ASoldierCharacter::MoveForward(const FInputActionValue& InputValue)
{
	if (bIsReloading) return;
	const float ForwardValue =  InputValue.Get<float>();
	AddMovementInput(GetActorForwardVector() * ForwardValue);
}

void ASoldierCharacter::MoveRight(const FInputActionValue& InputValue)
{
	if (bIsReloading) return;
	const float RightdValue =  InputValue.Get<float>();
	AddMovementInput(GetActorRightVector() * RightdValue);
}

void ASoldierCharacter::TurnCameraView(const FInputActionValue& InputValue)
{
	const FVector2D TurnCameraViewValue = InputValue.Get<FVector2D>();
	float TuneSpeed = 1;
	if (bInZoom)
		TuneSpeed = 5/(90-Guns[ActiveGunIndex]->FieldOfView); 
	AddControllerYawInput(TurnCameraViewValue.X * TuneSpeed);
	AddControllerPitchInput(-1*TurnCameraViewValue.Y * TuneSpeed);
}

AActor* ASoldierCharacter::Shoot()
{
	if (bIsFiring || bIsReloading) return nullptr;
	bIsFiring = true;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDel;
	TimerDel.BindLambda([&]()
		{bIsFiring = false;});
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDel, Guns[ActiveGunIndex]->TriggerIntervals,false);
	return Guns[ActiveGunIndex]->Shoot();
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
	// SetLifeSpan(7);
	// UnPossessed();
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
		if(Guns[ActiveGunIndex])
			Guns[ActiveGunIndex]->Destroy();
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

void ASoldierCharacter::ManageGuns()
{
	for (int i = 0; i <  Guns.Num(); i++)
	{
		if (i == ActiveGunIndex)
			Guns[i]->SetActorHiddenInGame(false);
		else
			Guns[i]->SetActorHiddenInGame(true);
	}
	SetTriggerIntervals();
}

void ASoldierCharacter::ChangeGun(const FInputActionValue& InputValue)
{
	float ScrolVal = InputValue.Get<float>();
	if(ScrolVal != 0 && bCanChangeGun)
	{
		ActiveGunIndex += ScrolVal;
		if(ActiveGunIndex >= Guns.Num())
			ActiveGunIndex = 0;
		if(ActiveGunIndex <= -1)
			ActiveGunIndex = Guns.Num() -1;
		ManageGuns();
	}
}

void ASoldierCharacter::RemoveSniperViewWidget()
{
	if (WBP_SniperView)
		if (WBP_SniperView->IsInViewport())
			WBP_SniperView->RemoveFromParent();
}

void ASoldierCharacter::Reload()
{
	bIsReloading = true;
	bCanChangeGun = false;
	FTimerHandle TimerHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]()
	{
		if(GunIsSafe())
				Guns[ActiveGunIndex]->Reload();
		bIsReloading = false;
		bCanChangeGun = true;
	});
	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate,Guns[ActiveGunIndex]->ReloadTime, false);
}

int ASoldierCharacter::GetMagAmmo()
{
	return Guns[ActiveGunIndex]->CurrentMagAmmo;
}

void ASoldierCharacter::Zoom()
{
	if (ActiveGunIndex != 2) return;
	if (!bInZoom)
	{
		bInZoom = true;
		bCanChangeGun = false;
		SpringArm->TargetArmLength = -1*Guns[ActiveGunIndex]->ZoomValue;
		Camera->FieldOfView = Guns[ActiveGunIndex]->FieldOfView;
		WBP_SniperView = CreateWidget<UUserWidget>(GetWorld(), SniperViewClass);
		if (WBP_SniperView)
			WBP_SniperView->AddToViewport();
	}
	else
	{
		bInZoom = false;
		bCanChangeGun = true;
		SpringArm->TargetArmLength = 200;
		Camera->FieldOfView = 90;
		RemoveSniperViewWidget();
	}
}

bool ASoldierCharacter::GunIsSafe()
{
	if(!Guns.IsEmpty() && ActiveGunIndex < Guns.Num())
		if(IsValid(Guns[ActiveGunIndex]))
			return true;
	return false;
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

