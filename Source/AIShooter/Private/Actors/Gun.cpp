// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Gun.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/DamageEvents.h" // For FPointDamageEvent
#include "Characters/SoldierCharacter.h"

#define OUT

void AGun::SetAmmo(int Ammo)
{
	if (Ammo > CurrentMagAmmo)
		CurrentAmmo = Ammo;
	else
	{
		CurrentAmmo = Ammo;
		CurrentMagAmmo = Ammo;
	}
}

void AGun::CreateRootComponent()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
}

void AGun::CreateGunMeshComponent()
{
	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Gun Mesh");
	GunMeshComponent->SetupAttachment(Root);
}

AGun::AGun()
{
	PrimaryActorTick.bCanEverTick = true;

	CreateRootComponent();
	CreateGunMeshComponent();
}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmo = MaxAmmo;
	CurrentMagAmmo = MaxMagAmmo;
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::HandleFiringEffects()
{
	FName TargetSocket;
	USkeletalMeshComponent* TargetMeshComponent;
	FRotator TargetRotation = FRotator(0.f);
	if (bHideMesh)
	{
		TargetMeshComponent = Cast<ASoldierCharacter>(GetOwner())->GetMesh();
		TargetSocket = FName("gun_barrel");
		TargetRotation = FRotator(-90,-90,0);
	}
	else
	{
		TargetMeshComponent = GunMeshComponent;
		TargetSocket = FName("MuzzleFlash");
	}
	if(FireSound)
		UGameplayStatics::SpawnSoundAttached(FireSound, TargetMeshComponent, TargetSocket, FVector(0),TargetRotation);
	for (UParticleSystem* FireParticle : FireParticles)
	{
		if(FireParticle)
			UGameplayStatics::SpawnEmitterAttached(FireParticle, TargetMeshComponent, TargetSocket,FVector(0),TargetRotation);
	}
}

void AGun::FindCameraPoint()
{
	if(APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		OwnerPawn->GetController()->GetPlayerViewPoint(OUT CameraLocation, OUT CameraRotation);
	}
	// DrawDebugCamera(GetWorld(), CameraLocation, CameraRotation, 90, 2,FColor::Red, false, 1);
}

void AGun::SetIgnoredActorsAsParams(FCollisionQueryParams& Params)
{
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	IgnoredActors.Add(GetOwner());
	Params.AddIgnoredActors(IgnoredActors);
}

bool AGun::ApplyBulletTrace(FHitResult& BulletHitResult)
{
	FCollisionQueryParams Params;
	SetIgnoredActorsAsParams(Params);
	FVector TraceEnd = CameraLocation + CameraRotation.Vector() * TraceRange;
	bool HitSomething = GetWorld()->LineTraceSingleByChannel(OUT BulletHitResult, CameraLocation, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1, Params);
	return HitSomething;
}

void AGun::HandleBodyImpactEffects (FHitResult const& BulletHitResult)
{
	if(ImpactSurfaceSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactBodySound, BulletHitResult.ImpactPoint);
	for (UParticleSystem* ImpactBodyParticle : ImpactBodyParticles)
	{
		if(ImpactBodyParticle)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactBodyParticle, BulletHitResult.ImpactPoint, BulletHitResult.ImpactNormal.Rotation());
	}
}

void AGun::HandleSurfaceImpactEffects(FHitResult const& BulletHitResult)
{
	if(ImpactSurfaceSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSurfaceSound, BulletHitResult.ImpactPoint);
	for (UParticleSystem* ImpactSurfaceParticle : ImpactSurfaceParticles)
	{
		if(ImpactSurfaceParticle)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactSurfaceParticle, BulletHitResult.ImpactPoint, BulletHitResult.ImpactNormal.Rotation());
	}
}

void AGun::HandleRadialDamage(FHitResult const& BulletHitResult)
{
	TArray<AActor*> IgnoredActors;
	if (TargetDamageType == EDamageType::RadialDamage)
	{
		UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, BulletHitResult.ImpactPoint, DamageRadius, DamageType, IgnoredActors, this);
	}
	else if (TargetDamageType == EDamageType::RadialDamageWithFalloff)
		UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), Damage, MinimumDamage, BulletHitResult.ImpactPoint,
			DamageInnerRadius, DamageOuterRadius, DamageFalloff, DamageType, IgnoredActors, this);
}

void AGun::HandleApplyDamage(FHitResult const& BulletHitResult, APawn* const& DamagedPawn)
{
	FVector ShotVector = BulletHitResult.ImpactPoint - CameraLocation;
	FVector ShotDirection = ShotVector.GetSafeNormal();
	AController* OwnerInstigator = Cast<APawn>(GetOwner())->GetController();

	//FDamageEvent DamageEvent();
	// FPointDamageEvent DamageEvent(Damage, BulletHitResult, ShotDirection, DamageType);
	// DamagedPawn->TakeDamage(Damage, DamageEvent, OwnerInstigator, this);
	
	if (TargetDamageType == EDamageType::PointDamage)
	{
		UGameplayStatics::ApplyPointDamage(DamagedPawn, Damage, ShotDirection, BulletHitResult, OwnerInstigator, this, DamageType);
	}
	else
		HandleRadialDamage(BulletHitResult);
}

bool AGun::Reload()
{
	if (CurrentAmmo == 0) return false;
	int dif = MaxMagAmmo - CurrentMagAmmo;
	if (dif == 0) return true;
	if (CurrentAmmo >= dif)
	{
		CurrentAmmo -= dif;
		CurrentMagAmmo = MaxMagAmmo;
	}
	else
	{
		CurrentMagAmmo += CurrentAmmo;
		CurrentAmmo = 0;
	}
	return true;
}

AActor* AGun::Shoot()
{
	if (CurrentMagAmmo <= 0) return nullptr;
	HandleFiringEffects();
	FindCameraPoint();
	FHitResult BulletHitResult;
	if(ApplyBulletTrace(BulletHitResult))
	{
		CurrentMagAmmo--;
		CurrentAmmo--;
		if (APawn* DamagedPawn = Cast<APawn>(BulletHitResult.GetActor()))
		{
			HandleBodyImpactEffects(BulletHitResult);
			HandleApplyDamage(BulletHitResult, DamagedPawn);
		}
		else
		{
			HandleSurfaceImpactEffects(BulletHitResult);
			HandleRadialDamage(BulletHitResult);
		}
		return BulletHitResult.GetActor();
	}
	return nullptr;
	// DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Yellow, false, 1, 0, 3);
}

void AGun::HideMesh(bool State)
{
	bHideMesh = State;
	GunMeshComponent->SetVisibility(false);
}

