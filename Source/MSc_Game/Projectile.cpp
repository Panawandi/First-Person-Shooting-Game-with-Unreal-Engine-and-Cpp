// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "EnemyAI.h"



// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollideSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Collision Sphere"));
	CollideSphere->InitSphereRadius(20.0f);

	RootComponent = CollideSphere;

	AmmoMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Ammo Movement"));
	AmmoMovement->UpdatedComponent = CollideSphere;
	AmmoMovement->MaxSpeed = 50000.0f;
	AmmoMovement->bRotationFollowsVelocity = true;
	AmmoMovement->InitialSpeed = 45000.0f;

	InitialLifeSpan = 2.0f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	CollideSphere->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnHit);
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	// Attempt to cast the "OtherActor" to the AEnemyAI class
	AEnemyAI* EnemyAI = Cast<AEnemyAI>(OtherActor);

	// Check if the cast was successful
	if (EnemyAI) {
		// If the cast was successful, call the AIDMG function on the EnemyAI instance
		EnemyAI->AIDMG(HP_DMGInput);
		// Destroy the projectile after applying damage to the enemy
		Destroy();
	}
}

