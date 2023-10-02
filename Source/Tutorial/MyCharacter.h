// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyInterface.h"
#include "MyCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKFOnCoinPickUp);

UCLASS()
class TUTORIAL_API AMyCharacter : public ACharacter, public IMyInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	UPROPERTY()
		FKFOnCoinPickUp OnCoinPickUp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move_XAxis(float AxisValue);

	void Move_YAxis(float AxisValue);

	void PitchCamera(float AxisValue);

	void YawCamera(float AxisValue);

	void DoJump();
	void StopJump();

	//Input variables
	FVector CurrentVelocity;
	FVector CameraInput;
	float myYaw;

	// FPS camera.
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCameraComponent;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
		USkeletalMesh* AlternateMeshAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		float DefaultHealth;

	UPROPERTY(VisibleAnywhere, Category = SkeletalMesh)
		class USkeletalMeshComponent* PlayerMesh;

	UFUNCTION()
		void myOnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
		void myOnCoinPickUp();

	void Interact_Implementation() override;

	virtual void InteractPure() override;

	//Timer
	int CallTracker = 3;

	void TimerFunction();

	void CheckPlayerCooldownTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
		FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool bInCooldown;
};
