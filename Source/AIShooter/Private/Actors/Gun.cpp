// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Gun.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/DamageEvents.h" // For FPointDamageEvent

#define OUT

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
	
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::FiringEffects()
{
	if(FireSound)
		UGameplayStatics::SpawnSoundAttached(FireSound, GunMeshComponent, FName("MuzzleSocketFlash"));
	for (UParticleSystem* FireParticle : FireParticles)
	{
		if(FireParticle)
			UGameplayStatics::SpawnEmitterAttached(FireParticle, GunMeshComponent, FName("MuzzleSocketFlash"));
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

bool AGun::ApplyBulletTrace(FHitResult& BulletHitResult)
{
	FVector TraceEnd = CameraLocation + CameraRotation.Vector() * TraceRange;
	bool HitSomething = GetWorld()->LineTraceSingleByChannel(OUT BulletHitResult, CameraLocation, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1);
	return HitSomething;
}

void AGun::HandleSurfaceImpactEffects (FHitResult const& BulletHitResult)
{
	if(ImpactSurfaceSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactBodySound, BulletHitResult.ImpactPoint);
	for (UParticleSystem* ImpactBodyParticle : ImpactBodyParticles)
	{
		if(ImpactBodyParticle)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactBodyParticle, BulletHitResult.ImpactPoint, BulletHitResult.ImpactNormal.Rotation());
	}
}

void AGun::HandleBodyImpactEffects(FHitResult const& BulletHitResult)
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

void AGun::Shoot()
{
	FiringEffects();
	FindCameraPoint();
	FHitResult BulletHitResult;
	if(ApplyBulletTrace(BulletHitResult))
	{
		if (APawn* DamagedPawn = Cast<APawn>(BulletHitResult.GetActor()))
		{
			HandleSurfaceImpactEffects(BulletHitResult);
			HandleApplyDamage(BulletHitResult, DamagedPawn);
		}
		else
		{
			HandleBodyImpactEffects(BulletHitResult);
			HandleRadialDamage(BulletHitResult);
		}
	}

	// DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Yellow, false, 1, 0, 3);
}

