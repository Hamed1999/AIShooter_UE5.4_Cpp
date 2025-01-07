// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Gun.h"

#include "DrawDebugHelpers.h"

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
	FVector CameraLocation = FVector().Zero();
	FRotator CameraRotation = FRotator().ZeroRotator;
	if(APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		OwnerPawn->GetController()->GetPlayerViewPoint(OUT CameraLocation, OUT CameraRotation);
	}
	DrawDebugCamera(GetWorld(), CameraLocation, CameraRotation, 90, 2,FColor::Red, false, 1);

	FHitResult BulletHitResult;
	FVector TraceEnd = CameraLocation + CameraRotation.Vector() * TraceRange;
	GetWorld()->LineTraceSingleByChannel(OUT BulletHitResult, CameraLocation, TraceEnd, ECollisionChannel::ECC_EngineTraceChannel1);

	DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Yellow, false, 1, 0, 3);
}

