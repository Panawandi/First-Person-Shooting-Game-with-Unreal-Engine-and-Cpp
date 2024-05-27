// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAI.h"
#include "Components/BoxComponent.h"
#include "MyPlayer.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AEnemyAI::AEnemyAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DMGCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DMGCollision->SetupAttachment(RootComponent);

	EnemyAIComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Enemy AI Perception Component"));
	SenseSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Sense Config"));

	SenseSight->SightRadius = 1350.0f;
	SenseSight->LoseSightRadius = 1380.0f;
	SenseSight->PeripheralVisionAngleDegrees = 90.0f;
	SenseSight->DetectionByAffiliation.bDetectEnemies = true;
	SenseSight->DetectionByAffiliation.bDetectFriendlies = true;
	SenseSight->DetectionByAffiliation.bDetectNeutrals = true;
	SenseSight->SetMaxAge(0.1f);

	EnemyAIComp->ConfigureSense(*SenseSight);
	EnemyAIComp->SetDominantSense(SenseSight->GetSenseImplementation());
	EnemyAIComp->OnPerceptionUpdated.AddDynamic(this, &AEnemyAI::OnSensed);

	Velocity = FVector::ZeroVector;
	EnemySpeed = 485.0f;
	bReturning = false;

}

// Called when the game starts or when spawned
void AEnemyAI::BeginPlay()
{
	Super::BeginPlay();

	DMGCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyAI::OnHit);
	
	Location = this->GetActorLocation();

}

// Called every frame
void AEnemyAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Velocity.IsZero()) {

		FVector NewLocation = GetActorLocation() + Velocity * DeltaTime;

		if (bReturning) {
			if ((NewLocation - Location).SizeSquared2D() < 25.0f) {
				Velocity = FVector::ZeroVector;
				ReturnLocation = false;

				SetNewRotation(GetActorForwardVector(), GetActorLocation());
			}
		}
		
		SetActorLocation(NewLocation);
	}

}

// Function to move the enemy to a specified location and return back
void AEnemyAI::MoveToLocation(FVector TargetLocation) {
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	Velocity = Direction * EnemySpeed;

	Location = GetActorLocation();
	bReturning = true;
}

// Called to bind functionality to input
void AEnemyAI::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyAI::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	// Cast the "OtherActor" to the AMyPlayer class
	AMyPlayer* Char = Cast<AMyPlayer>(OtherActor);

	// Check if the cast was successful
	if (Char) {
		// If the cast was successfull, call the HP_Damage function on the AMyPlayer Instance
		Char->HP_Damage(EnemyDMG);
	}
}

void AEnemyAI::OnSensed(const TArray<AActor*>& UpdateActors)
{
	for (AActor* Actor : UpdateActors) {
	
		FActorPerceptionBlueprintInfo Info;
		EnemyAIComp->GetActorsPerception(Actor, Info);

		if (Info.LastSensedStimuli.IsValidIndex(0) && Info.LastSensedStimuli[0].WasSuccessfullySensed()) {
			FVector Direction = Actor->GetActorLocation() - GetActorLocation();
			Direction.Z = 0.0f;

			Velocity = Direction.GetSafeNormal() * EnemySpeed;

			SetNewRotation(Actor->GetActorLocation(), GetActorLocation());
		}
		else
		{
			FVector Direction = Location - GetActorLocation();
			Direction.Z = 0.0f;

			if (Direction.SizeSquared2D() > 1.0f) {
				Velocity = Direction.GetSafeNormal() * EnemySpeed;
				ReturnLocation = true;

				SetNewRotation(Location, GetActorLocation());
			}
		}
	}
}

void AEnemyAI::SetNewRotation(FVector TargetPosition, FVector CurrentPosition)
{

	FVector NewDirection = TargetPosition - CurrentPosition;
	NewDirection.Z = 0.0f;

	Rotation = NewDirection.Rotation();

	SetActorRotation(Rotation);

}

void AEnemyAI::AIDMG(float DamageAmount)
{
	EnemyHP -= DamageAmount;
	if (IsDead()) {
		Destroy();
	}
}

bool AEnemyAI::IsDead() const {
	return EnemyHP <= 0.0f;
}