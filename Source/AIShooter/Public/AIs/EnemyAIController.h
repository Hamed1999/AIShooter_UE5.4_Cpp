// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyAIController();
	virtual void Tick(float DeltaTime) override;

protected:
	/**
	 * Methods
	 */
	void CreateSightSenseConfig();
	void CreateAIPerception();
	void SetTeamId();
	UFUNCTION(BlueprintType)
		void OnPlayerSeen(AActor* Actor, struct FAIStimulus Stimulus);
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category= "Properties", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UBehaviorTree* BT_EnemyBehavior;
	UPROPERTY(EditAnywhere, Category= "Properties", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditAnywhere, Category= "Properties", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		class UAISenseConfig_Sight* SenseConfig_Sight;
	UPROPERTY(EditAnywhere, Category= "Properties", BlueprintReadWrite, Meta = (AllowPrivateAccess))
		FGenericTeamId TeamId = FGenericTeamId(1);
};