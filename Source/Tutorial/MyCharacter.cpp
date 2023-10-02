// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	if (AlternateMeshAsset)
	{
		PlayerMesh->SetSkeletalMesh(AlternateMeshAsset);
	}
	// Attach the camera component to our capsule component.
	CameraComp->SetupAttachment(GetMesh());
	//CameraComp->SetupAttachment(GetCapsuleComponent());
	// Position the camera slightly above the eyes.
	CameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	// Allow the pawn to control camera rotation.
	//CameraComp->bUsePawnControlRotation = true;

	SpringArmComp->SetupAttachment(GetMesh());
	//SpringArmComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//Setting Default Variables and Behavior of the SpringArmComponent
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.0f;

	//Take control of the default Player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Air control
	GetCharacterMovement()->AirControl = 1.0f;

	//Ontakeanydamage delegate
	OnTakeAnyDamage.AddDynamic(this, &AMyCharacter::myOnTakeAnyDamage);
	//OnTakeAnyDamage.AddDynamic(this, FName(TEXT("myOnTakeAnyDamage")));

	OnCoinPickUp.AddDynamic(this, &AMyCharacter::myOnCoinPickUp);

	DefaultHealth = 100.f;
	Health = DefaultHealth;

	bInCooldown = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Interface call here
	//InteractPure();

	//Invoke delegate function
	//OnCoinPickUp.Broadcast();

	//Rotate our actor's yaw, which will turn our camera because we're attached to it
	{
		//DEBUG WIT THIS
		/*
		int intVar = 5;
		float floatVar = CurrentVelocity.X;
		//float floatVar = CurrentVelocity.Y;
		FString fstringVar = "an fstring variable";
		UE_LOG(LogTemp, Warning, TEXT("Text, %d %f %s"), intVar, myYaw, *fstringVar);
		*/

		myYaw += CameraInput.X;
		GetCapsuleComponent()->SetRelativeRotation(FRotator(0.0f, myYaw, 0.0f), false);
		AddMovementInput(GetControlRotation().Yaw * FVector(1.0, 0.0, 0.0), CurrentVelocity.X, true);
		AddMovementInput(GetControlRotation().Yaw * FVector(1.0, 0.0, 0.0), CurrentVelocity.Y, true);
	}

	//Rotate our camera's pitch, but limit it so we're always looking downward
	{
		FRotator NewRotation = SpringArmComp->GetComponentRotation();
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
		SpringArmComp->SetWorldRotation(NewRotation);
	}

	if (!CurrentVelocity.IsZero())
	{
		if (GEngine)
		{
			// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Movement"));
			AddMovementInput(GetCapsuleComponent()->GetForwardVector(), CurrentVelocity.X, true);
			AddMovementInput(GetCapsuleComponent()->GetRightVector(), CurrentVelocity.Y, true);
		}
	}

	if (CurrentVelocity.IsZero())
	{
		if (GEngine)
		{
			// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No input"));

		}
	}

	CheckPlayerCooldownTimer();
}

void AMyCharacter::Move_XAxis(float AxisValue)
{
	// Move at 100 units per second forward or backward
	CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AMyCharacter::Move_YAxis(float AxisValue)
{
	// Move at 100 units per second right or left
	CurrentVelocity.Y = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 100.0f;
}

void AMyCharacter::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void AMyCharacter::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void AMyCharacter::DoJump()
{
	if (CanJump())
	{
		Jump();
	}
}

void AMyCharacter::StopJump()
{
	StopJumping();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Respond every frame to the values of our two movement axes, "MoveX" and "MoveY".
	InputComponent->BindAxis("MoveX", this, &AMyCharacter::Move_XAxis);
	InputComponent->BindAxis("MoveY", this, &AMyCharacter::Move_YAxis);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::DoJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AMyCharacter::StopJump);
	InputComponent->BindAxis("CameraPitch", this, &AMyCharacter::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &AMyCharacter::YawCamera);
}

void AMyCharacter::myOnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	//In Cooldown no damage
	if (!bInCooldown)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyCharacter::TimerFunction, 3.0f);
		Health -= Damage;

		FString TheFloatStr = FString::SanitizeFloat(Damage);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, *TheFloatStr);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Do Damage"));
	}
	//If the timer is active
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("No Damage"));
	}
	
}

void AMyCharacter::CheckPlayerCooldownTimer()
{
	if (GetWorldTimerManager().IsTimerActive(TimerHandle))
	{
		bInCooldown = true;
	}
	else
	{
		bInCooldown = false;
	}
}

void AMyCharacter::myOnCoinPickUp()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CoinPickUp"));

}

//Interface
void AMyCharacter::Interact_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Interact BP"));
}

//Interface
void AMyCharacter::InteractPure()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Interact"));
}

void AMyCharacter::TimerFunction()
{
	CallTracker--;

	if (CallTracker == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Timer Finished"));
		GetWorldTimerManager().ClearTimer(TimerHandle);
		bInCooldown = false;
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Timer Called"));
		bInCooldown = true;
	}
}

//Delegate

/*
FVector AMyCharacter::MyGetPlayerLocation()
{
	return GetActorLocation();
}
*/