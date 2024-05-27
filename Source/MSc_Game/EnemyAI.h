// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyAI.generated.h"

UCLASS()
class MSC_GAME_API AEnemyAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = EnemyAI)
		void MoveToLocation(FVector TargetLocation);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		class UBoxComponent* DMGCollision;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit);

	//UAISenseConfig is to configure AI perception component for sight detection
	UPROPERTY(VisibleDefaultsOnly, Category = EnemyAI)
		class UAISenseConfig_Sight* SenseSight;

	// UAIPerceptionComponents implements senses for an actor, like hearings or sight
	UPROPERTY(VisibleDefaultsOnly, Category = EnemyAI)
		class UAIPerceptionComponent* EnemyAIComp;

	//this function will inform if any actors are detected by the UAIPerceptionComponent
	UFUNCTION()
		void OnSensed(const TArray<AActor*>& UpdateActors);

	UPROPERTY(VisibleAnywhere, Category = Movement)
		float EnemySpeed;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		FRotator Rotation;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		FVector Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		FVector Velocity;

	void SetNewRotation(FVector TargetPosition, FVector CurrentPosition);

	bool ReturnLocation;
	bool IsDead() const;
	FVector NewLocation;
	float DistanceSquared;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float EnemyHP = 100.0f;

	UPROPERTY(EditAnywhere)
		float EnemyDMG = 35.0f;

public:
	void AIDMG(float DamageAmount);

private:
	bool bReturning;
};
