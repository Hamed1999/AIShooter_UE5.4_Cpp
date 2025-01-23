// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Reload.generated.h"

/**
 * 
 */
UCLASS()
class AISHOOTER_API UBTTaskNode_Reload : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTaskNode_Reload(const FObjectInitializer& ObjectInitializer);
private:
	virtual  EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
