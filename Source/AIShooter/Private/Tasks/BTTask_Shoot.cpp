// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTTask_Shoot.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/SoldierCharacter.h"

UBTTask_Shoot::UBTTask_Shoot(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ASoldierCharacter* TargetSoldier = Cast<ASoldierCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Player")));
	if (AAIController* Controller = OwnerComp.GetAIOwner())
		if (ASoldierCharacter* SoldierCharacter = Cast<ASoldierCharacter>(Controller->GetPawn()))
			if (TargetSoldier)
			{
				if (TargetSoldier->IsDead())
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsDead"),true);
					return EBTNodeResult::Failed;
				}
				SoldierCharacter->Shoot();
				return EBTNodeResult::Succeeded;
			}
	return EBTNodeResult::Failed;
}
