// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEnemyBase.h"

// Sets default values
AMyEnemyBase::AMyEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OnTakeAnyDamage.AddDynamic(this, &AMyEnemyBase::myOnTakeAnyDamage);
}

// Called when the game starts or when spawned
void AMyEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	//getting actors by tag
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Player", FoundActors);

	//getting components by tag
	//TArray myCapsuleArray = GetOwner()->GetComponentsByTag(UCapsuleComponent::StaticClass(), FName("Player"));

	CapsuleComponents = GetOwner()->GetComponentsByTag(UCapsuleComponent::StaticClass(), FName("Player"));

	AIController = Cast<AMyAIController>(GetController());
	GetController()->GetPawn()->AIControllerClass = AMyAIController::StaticClass();
	PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyEnemyBase::OnBoxBeginOverlap);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AMyEnemyBase::OnBoxBeginHit);
	//GetCapsuleComponent()->OnComponentHit.Add(&AMyEnemyBase::OnBoxBeginHit);
}

// Called every frame
void AMyEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AnimalNoise();
	EnumBehavior();

	/*
	if (CapsuleComponents.Num() < 1)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("NONE"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("1"));
	}
	*/

	//learned typecasting

	//GetByActor using component by its class
	AIController->MoveToLocation(Cast<UCapsuleComponent>(FoundActors[0]->GetComponentByClass(UCapsuleComponent::StaticClass()))->GetRelativeLocation(), -1.0f, true, false);

	//GetByComponent using component by its class
	//AIController->MoveToLocation(Cast<UCapsuleComponent>(CapsuleComponents[0])->GetRelativeLocation(), -1.0f, true, false);
}

// Called to bind functionality to input
void AMyEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyEnemyBase::AnimalNoise()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Bark"));
}

void AMyEnemyBase::EnumBehavior()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Enum"));
}

void AMyEnemyBase::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Overlapping"));
}

void AMyEnemyBase::OnBoxBeginHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Hit"));
		//AIController->bUsePathFinding = false;

		UGameplayStatics::ApplyDamage(OtherActor, 10.0f, GetController(), this, NULL);

	/*
		//get Location and Roation of PlayerCharacter
		//FVector PlayerPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		//FRotator PlayerRotation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();
		FVector PlayerPosition = PlayerPawn->GetActorLocation();
		FRotator PlayerRotation = PlayerPawn->GetActorRotation();

		FVector NewLocation = FVector(PlayerPosition.X - 500.f, PlayerPosition.Y - 500.f, PlayerPosition.Z);
		float Var = 100.f;

		SetActorRotation(PlayerRotation);
		AIController->WalkAway(NewLocation, Var);
		//SetActorLocation(NewLocation);


		UE_LOG(LogTemp, Warning, TEXT("MyCharacter's Location is %s"), *OtherActor->GetName());
	*/
	}

}

void AMyEnemyBase::myOnTakeAnyDamage(AActor * DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor * DamageCauser)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("DAMAGE"));

}