// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class AISHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:
	void CreateRootComponent();
	void CreateGunMeshComponent();
	AGun();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void Shoot();

private:
	/**
	 *Global Properties
	 */
	UPROPERTY(EditAnywhere, Category= "Components", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		USceneComponent* Root;
	UPROPERTY(EditAnywhere, Category= "Components", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		USkeletalMeshComponent* GunMeshComponent;
	UPROPERTY(EditAnywhere, Category= "Properties", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		float TraceRange = 45000;

};
