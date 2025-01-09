// Fill out your copyright notice in the Description page of Project Settings.


#include "AIs/EnemyAIController.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(TEXT("/Game/AI/BT_EnemyBehavior"));
	if(BehaviorTreeRef.Succeeded())
		BT_EnemyBehavior = BehaviorTreeRef.Object.Get();
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	//MoveToLocation(FVector(2939.697232,-9.828129,134.400113));
	// SetFocus(PlayerPawn, EAIFocusPriority::Gameplay);
	RunBehaviorTree(BT_EnemyBehavior);
	GetBlackboardComponent()->SetValueAsVector(FName("Destination"), PlayerPawn->GetActorLocation());
	GetBlackboardComponent()->SetValueAsObject(FName("Player"), PlayerPawn);
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (LineOfSightTo(PlayerPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("In sight : )"))
		GetBlackboardComponent()->SetValueAsObject(FName("Player"), PlayerPawn);
		SetFocus(PlayerPawn, EAIFocusPriority::Gameplay);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Out of Sight !"))
		GetBlackboardComponent()->ClearValue(FName("Player"));
		SetFocus(nullptr, EAIFocusPriority::Gameplay);
	}
}