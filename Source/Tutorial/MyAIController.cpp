// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"

void AMyAIController::BeginPlay()
{
    Super::BeginPlay();
}

void AMyAIController::WalkAway(FVector Location, float Var)
{

    MoveToLocation(Location, Var);
}