// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyBat.h"

/*
void AMyEnemyBat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AnimalNoise();
}

*/

void AMyEnemyBat::AnimalNoise()
{
	//MovementEnum = NONE;
	//Super::AnimalNoise();
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Woof"));
}

void AMyEnemyBat::EnumBehavior()
{
	if (MovementEnum == NONE)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NONE"));
	} 
	else if (MovementEnum == HOMING)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("HOMING"));
		AActor* PlayerActor = FoundActors[0];
		AddMovementInput(PlayerActor->GetActorLocation(), 100.0f, true);
	}
}