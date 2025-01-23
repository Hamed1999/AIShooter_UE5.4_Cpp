// Fill out your copyright notice in the Description page of Project Settings.


#include "Tasks/BTTaskNode_Reload.h"

#include "AIController.h"
#include "Characters/SoldierCharacter.h"
#include "Kismet/GameplayStatics.h"

UBTTaskNode_Reload::UBTTaskNode_Reload(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	NodeName = "Reload Gun";
}

EBTNodeResult::Type UBTTaskNode_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	UE_LOG(LogTemp, Error, TEXT("Reload Was Invoked"))
	if(ASoldierCharacter* SoldierCharacter = Cast<ASoldierCharacter>(Controller->GetCharacter()))
	{
		UE_LOG(LogTemp, Error, TEXT("Befor Reload Ammo = %i"),SoldierCharacter->GetAmmo())
		SoldierCharacter->Reload();
		if (SoldierCharacter->GetAmmo() > 0)
			return EBTNodeResult::Succeeded;
		UE_LOG(LogTemp, Error, TEXT("After Reload Ammo = %i"),SoldierCharacter->GetAmmo())
	}
	UE_LOG(LogTemp, Error, TEXT("Reload Failed .. . ."))
	return EBTNodeResult::Failed;
}
