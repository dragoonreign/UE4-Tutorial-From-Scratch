// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyEnemyBase.h"
#include "MyEnemyBat.generated.h"

/**
 * 
 */
UCLASS()
class TUTORIAL_API AMyEnemyBat : public AMyEnemyBase
{
	GENERATED_BODY()

public:
	
	/*
	*/
	virtual void AnimalNoise() override;

	virtual void EnumBehavior() override;
};
