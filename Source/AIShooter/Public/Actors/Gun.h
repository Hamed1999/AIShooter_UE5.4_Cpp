// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	PointDamage UMETA(DisplayName = "Point Damage"),
	RadialDamage UMETA(DisplayName = "Auto Radial Damage"),
	RadialDamageWithFalloff UMETA(DisplayName = "Radial Damage With Falloff"),
};

UCLASS()
class AISHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:
	AGun();

protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	AActor* Shoot();
	void HideMesh(bool State);
	UPROPERTY(EditAnywhere, Category= "Properties | Gun Type | Trigger", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0.1"))
		float TriggerIntervals = 0.2;
	UPROPERTY(EditAnywhere, Category= "Properties | Range", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0"))
		float ZoomValue = 150;
	UPROPERTY(EditAnywhere, Category= "Properties | Range", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "5"))
		float FieldOfView = 8;
	bool Reload();
	int CurrentMagAmmo;
	int CurrentAmmo;
	UPROPERTY(EditAnywhere, Category= "Ammo", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0.3"))
		float ReloadTime = 1.3;
	void SetAmmo(int Ammo);
private:
	/**
	 * Methods
	 */
	void CreateRootComponent();
	void CreateGunMeshComponent();
	void HandleFiringEffects();
	void FindCameraPoint();
	void SetIgnoredActorsAsParams(FCollisionQueryParams& Params);
	bool ApplyBulletTrace(FHitResult& BulletHitResult);
	void HandleBodyImpactEffects(FHitResult const&  BulletHitResult);
	void HandleSurfaceImpactEffects(FHitResult const& BulletHitResult);
	void HandleRadialDamage(FHitResult const& BulletHitResult);
	void HandleApplyDamage(FHitResult const& BulletHitResult, APawn* const& DamagedPawn);
	/**
	 *Global Properties
	 */
	// Components
	UPROPERTY(EditAnywhere, Category= "Components", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		USceneComponent* Root;
	UPROPERTY(EditAnywhere, Category= "Components", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		USkeletalMeshComponent* GunMeshComponent;
	// Properties
	UPROPERTY(EditAnywhere, Category= "Properties | Range", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		float TraceRange = 45000;
	FVector CameraLocation = FVector().Zero();
	FRotator CameraRotation = FRotator().ZeroRotator;
	UPROPERTY(EditAnywhere, Category= "Properties | Damage | Base", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		float Damage = 12;
	UPROPERTY(EditAnywhere, Category= "Properties | Damage | Base", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		TSubclassOf<UDamageType> DamageType;
	UPROPERTY(EditAnywhere, Category= "Properties | Damage | Base", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0"))
		EDamageType TargetDamageType = EDamageType::PointDamage;
	UPROPERTY(EditAnywhere, Category= "Properties | Damage | Auto Radial", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0"))
		float DamageRadius = 2000;
	UPROPERTY(EditAnywhere, Category= "Properties | Damage | Radial With Falloff", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0"))
		float MinimumDamage = 0.1;
	UPROPERTY(EditAnywhere, Category= "Properties | Damage | Radial With Falloff", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0"))
		float DamageInnerRadius = 300;
	UPROPERTY(EditAnywhere, Category= "Properties | Damage | Radial With Falloff", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0"))
		float DamageOuterRadius = 1000;
	bool bHideMesh = false;
	UPROPERTY(EditAnywhere, Category= "Ammo", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0"))
	 int MaxAmmo = 144;
	UPROPERTY(EditAnywhere, Category= "Ammo", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0"))
		int MaxMagAmmo = 24;
	
	/**
	 * 1 is linear.
	 * Greater than 1 cause damage to drop more rapidly between the inner and outer radii.
	 */
	UPROPERTY(EditAnywhere, Category= "Properties | Damage | Radial With Falloff", BlueprintReadWrite, Meta = (AllowPrivateAccess, ClampMin = "0"))
		float DamageFalloff = 1.0;
	// Effects
	UPROPERTY(EditAnywhere, Category= "Effects", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		USoundBase* FireSound = nullptr;
	UPROPERTY(EditAnywhere, Category= "Effects", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		TArray<UParticleSystem*> FireParticles;
	UPROPERTY(EditAnywhere, Category= "Effects", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		USoundBase* ImpactSurfaceSound = nullptr;
	UPROPERTY(EditAnywhere, Category= "Effects", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		USoundBase* ImpactBodySound = nullptr;
	UPROPERTY(EditAnywhere, Category= "Effects", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		TArray<UParticleSystem*> ImpactSurfaceParticles;
	UPROPERTY(EditAnywhere, Category= "Effects", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		TArray<UParticleSystem*> ImpactBodyParticles;
};
