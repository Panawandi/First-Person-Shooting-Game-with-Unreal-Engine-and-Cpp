// Fill out your copyright notice in the Description page of Project Settings.


#include "EndLine.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

#include "MyPlayer.h"
#include "FPGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEndLine::AEndLine()
{
	PrimaryActorTick.bCanEverTick = true;

	Goal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Goal"));
	RootComponent = Goal;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	CollisionComponent->SetupAttachment(Goal);

}

// Called when the game starts or when spawned
void AEndLine::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEndLine::OnHit);
	
}

// Called every frame
void AEndLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEndLine::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (AMyPlayer* Player = Cast<AMyPlayer>(OtherActor)) {
		if (AFPGameMode* TheGameMode = Cast<AFPGameMode>(UGameplayStatics::GetGameMode(GetWorld()))) {
			TheGameMode->GameReset(true);
		}
	}
}

