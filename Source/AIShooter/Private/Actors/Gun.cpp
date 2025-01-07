// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Gun.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

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

void AGun::Shoot()
{
	if(FireSound)
		UGameplayStatics::SpawnSoundAttached(FireSound, GunMeshComponent, FName("MuzzleSocketFlash"));
	for (UParticleSystem* FireParticle : FireParticles)
	{
		if(FireParticle)
			UGameplayStatics::SpawnEmitterAttached(FireParticle, GunMeshComponent, FName("MuzzleSocketFlash"));
	}
	
	FVector CameraLocation = FVector().Zero();
	FRotator CameraRotation = FRotator().ZeroRotator;
	if(APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		OwnerPawn->GetController()->GetPlayerViewPoint(OUT CameraLocation, OUT CameraRotation);
	}
	// DrawDebugCamera(GetWorld(), CameraLocation, CameraRotation, 90, 2,FColor::Red, false, 1);

	FHitResult BulletHitResult;
	FVector TraceEnd = CameraLocation + CameraRotation.Vector() * TraceRange;
	if(GetWorld()->LineTraceSingleByChannel(OUT BulletHitResult, CameraLocation, TraceEnd, ECollisionChannel::ECC_GameTraceChannel1))
	{
		if (APawn* DamagedPawn = Cast<APawn>(BulletHitResult.GetActor()))
		{
			if(ImpactSurfaceSound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactBodySound, BulletHitResult.ImpactPoint);
			for (UParticleSystem* ImpactBodyParticle : ImpactBodyParticles)
			{
				if(ImpactBodyParticle)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactBodyParticle, BulletHitResult.ImpactPoint, BulletHitResult.ImpactNormal.Rotation());
			}
		}
		else
		{
			if(ImpactSurfaceSound)
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSurfaceSound, BulletHitResult.ImpactPoint);
			for (UParticleSystem* ImpactSurfaceParticle : ImpactSurfaceParticles)
			{
				if(ImpactSurfaceParticle)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactSurfaceParticle, BulletHitResult.ImpactPoint, BulletHitResult.ImpactNormal.Rotation());
			}
		}
	}

	// DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Yellow, false, 1, 0, 3);
}

