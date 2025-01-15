// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTTask_Shoot.h"
#include "AIController.h"
#include "Characters/SoldierCharacter.h"

UBTTask_Shoot::UBTTask_Shoot(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	NodeName = "Shoot";
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAIController* Controller = OwnerComp.GetAIOwner())
		if (ASoldierCharacter* SoldierCharacter = Cast<ASoldierCharacter>(Controller->GetPawn()))
			if (ASoldierCharacter* TargetSoldier = Cast<ASoldierCharacter>(SoldierCharacter->Shoot()))
			{
				if (TargetSoldier->IsDead())
					return EBTNodeResult::Failed;
				
				return EBTNodeResult::Succeeded;
			}
			else
			{
				return EBTNodeResult::Failed;
			}
	return EBTNodeResult::Failed;
}
