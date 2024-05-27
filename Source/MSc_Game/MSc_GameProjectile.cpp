// Copyright Epic Games, Inc. All Rights Reserved.

#include "MSc_GameProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AMSc_GameProjectile::AMSc_GameProjectile() 
{
	// Use a sphere as a simple collision representation
	OnHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	OnHitComp->InitSphereRadius(5.0f);
	OnHitComp->BodyInstance.SetCollisionProfileName("Projectile");
	OnHitComp->OnComponentHit.AddDynamic(this, &AMSc_GameProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	OnHitComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	OnHitComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = OnHitComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	BulletSpeed = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	BulletSpeed->UpdatedComponent = OnHitComp;
	BulletSpeed->InitialSpeed = 3000.f;
	BulletSpeed->MaxSpeed = 3000.f;
	BulletSpeed->bRotationFollowsVelocity = true;
	BulletSpeed->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AMSc_GameProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}