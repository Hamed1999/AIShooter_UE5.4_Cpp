// Fill out your copyright notice in the Description page of Project Settings.


#include "AIs/EnemyAIController.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/SoldierCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionTypes.h"

void AEnemyAIController::CreateSightSenseConfig()
{
	SenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Sense config"));
	SenseConfig_Sight->SightRadius = 6500;
	SenseConfig_Sight->LoseSightRadius = 10000;
	SenseConfig_Sight->PeripheralVisionAngleDegrees = 70;
	SenseConfig_Sight->DetectionByAffiliation = FAISenseAffiliationFilter(true, false, false);
}

void AEnemyAIController::CreateAIPerception()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AI Perception Component"));
	AIPerceptionComponent->ConfigureSense(*SenseConfig_Sight);
}

void AEnemyAIController::SetTeamId()
{
	if (ASoldierCharacter* OwnerCharacter = Cast<ASoldierCharacter>(GetPawn()))
		TeamId = OwnerCharacter->GetGenericTeamId();
		SetGenericTeamId(TeamId);
}

void AEnemyAIController::OnPlayerSeen(AActor* Actor, FAIStimulus Stimulus)
{
	// UE_LOG(LogTemp, Error, TEXT("%s Is Seen : )"), *Actor->GetName())
	if (Stimulus.WasSuccessfullySensed())
	{
		//UE_LOG(LogTemp, Error, TEXT("In sight : )"))
		GetBlackboardComponent()->SetValueAsObject(FName("Player"),Actor);
		SetFocus(Actor, EAIFocusPriority::Gameplay);
	}
	else
	{
		// UE_LOG(LogTemp, Error, TEXT("Out of Sight !"))
		GetBlackboardComponent()->ClearValue(FName("Player"));
		ClearFocus(EAIFocusPriority::Gameplay);
		GetBlackboardComponent()->SetValueAsVector(FName("LastSeenLocation"), Actor->GetActorLocation());
	}
}

AEnemyAIController::AEnemyAIController()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTreeRef(TEXT("/Game/AI/BT_EnemyBehavior"));
	if(BehaviorTreeRef.Succeeded())
		BT_EnemyBehavior = BehaviorTreeRef.Object.Get();
	CreateSightSenseConfig();
	CreateAIPerception();
	SetTeamId();
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPlayerSeen);
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	SetTeamId();
	RunBehaviorTree(BT_EnemyBehavior);
	GetBlackboardComponent()->SetValueAsVector(FName("FirstLocation"), GetPawn()->GetActorLocation());
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}