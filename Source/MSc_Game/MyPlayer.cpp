// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "FPGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(40.0f, 95.0f);

	SideSens = 45.0f;
	UDSens = 45.0f;

	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraPOV"));
	Cam->SetupAttachment(GetCapsuleComponent());
	Cam->AddRelativeLocation(FVector(-39.65f, 1.75f, 64.9f));
	Cam->bUsePawnControlRotation = true;

	PlayerHands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player Hands"));

	PlayerHands->SetOnlyOwnerSee(true);
	PlayerHands->SetupAttachment(Cam);
	PlayerHands->bCastDynamicShadow = false;
	PlayerHands->CastShadow = false;
	PlayerHands->AddRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	PlayerHands->AddRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	Weapon = CreateAbstractDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetOnlyOwnerSee(true);
	Weapon->bCastDynamicShadow = false;
	Weapon->CastShadow = false;

	Location = CreateDefaultSubobject<USceneComponent>(TEXT("Location"));
	Location->SetupAttachment(Weapon);
	Location->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	WeaponOff = FVector(100.0f, 0.0f, 10.0f);
}

// Called when the game starts or when spawned
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Attach the gun mesh on the hands mesh
	// Grip point is where the gun is going to be attached to the hands

	Weapon->AttachToComponent(PlayerHands, FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("PointOfGrip"));
	
	World = GetWorld();
	AnimInstance = PlayerHands->GetAnimInstance();

}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setting up input component

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyPlayer::OnCrouch);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyPlayer::Shoots);


	PlayerInputComponent->BindAxis("WalkingStraight", this, &AMyPlayer::WalkStraight);
	PlayerInputComponent->BindAxis("WalkingSideways", this, &AMyPlayer::WalkingSideways);

	PlayerInputComponent->BindAxis("Turn", this, &AMyPlayer::SideSensTurn);
	PlayerInputComponent->BindAxis("LookUp", this, &AMyPlayer::UDSensTurn);
}

void AMyPlayer::Shoots()
{
	if (World != NULL) {
		PlayerRotate = GetControlRotation();
		PlayerSpawn = ((Location != nullptr) ? Location->GetComponentLocation() : GetActorLocation()) + PlayerRotate.RotateVector(WeaponOff);

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		World->SpawnActor<AProjectile>(Ammo, PlayerSpawn, PlayerRotate, ActorSpawnParams);

		if (ShotSE != NULL) {
			UGameplayStatics::PlaySoundAtLocation(this, ShotSE, GetActorLocation());
		}

		if (AmmoMovement != NULL && AnimInstance != NULL) {
			AnimInstance->Montage_Play(AmmoMovement, 1.0f);
		}
	}
}

void AMyPlayer::SetCrouchWalkingSpeed(float NewSpeed) {
	if (GetCharacterMovement()) {
		GetCharacterMovement()->MaxWalkSpeedCrouched = NewSpeed;
	}
}

void AMyPlayer::HP_Damage(float HP_DMGInput)
{
	Player_HP -= HP_DMGInput;

	(Player_HP <= 0.0f) ? (GetWorld()->GetAuthGameMode<AFPGameMode>()->GameReset(false), Destroy()) : nullptr;

}

// Failed attempt of binding crouch
void AMyPlayer::OnCrouch()
{
	if (!bIsCrouched) {
		Crouch();
		SetCrouchWalkingSpeed(150.0f);
	}
	else
	{
		UnCrouch();
		SetCrouchWalkingSpeed(600.0f);
	}
}

void AMyPlayer::WalkStraight(float x)
{
	x = x != 0.0f;
	AddMovementInput(GetActorForwardVector(), x);
	
}

void AMyPlayer::WalkingSideways(float x)
{
	if (x != 0.0f) {
		AddMovementInput(GetActorRightVector(), x);
	}
}

void AMyPlayer::SideSensTurn(float Turn)
{
	float DeltaTurn = Turn * SideSens * GetWorld()->GetDeltaSeconds();
	AddControllerYawInput(DeltaTurn);
}

void AMyPlayer::UDSensTurn(float Turn)
{
	AddControllerPitchInput(Turn * UDSens * GetWorld()->GetDeltaSeconds());
}
