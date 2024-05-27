// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyPlayer.generated.h"

UCLASS()
class MSC_GAME_API AMyPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	void Shoots();

	void WalkStraight(float x);
	void WalkingSideways(float x);

	void SideSensTurn(float Turn);
	void UDSensTurn(float Turn);

public:

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* PlayerHands;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* Location;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cam)
		class UCameraComponent* Cam;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cam)
		float SideSens;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Cam)
		float UDSens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Game)
		FVector WeaponOff;

public:

	void HP_Damage(float HP_DMGInput);

	void OnCrouch();

public:

	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		TSubclassOf<class AProjectile> Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* ShotSE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* AmmoMovement;

	UFUNCTION(BlueprintCallable, Category = Movement)
		void SetCrouchWalkingSpeed(float NewSpeed);

	class UAnimInstance* AnimInstance;

	class UWorld* World;

	FRotator PlayerRotate;
	FVector PlayerSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Player_HP = 100.0f;
};
