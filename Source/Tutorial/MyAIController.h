// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API AMyAIController : public AAIController
{
	GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION()
        void WalkAway(FVector Location, float Var);
};
