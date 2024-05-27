// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameMode.h"
#include "Kismet/GameplayStatics.h"

void AFPGameMode::GameReset(bool Victory)
{
	Victory ? StageRestart() : GetWorldTimerManager().SetTimerForNextTick(this, &AFPGameMode::StageRestart);
}

void AFPGameMode::StageRestart()
{
	UGameplayStatics::OpenLevel(GetWorld(), "FirstPersonMap");
}

void AFPGameMode::CDTime()
{
	(CountDown > 0) ? CountDown-- : (GetWorldTimerManager().ClearTimer(CountDownHandle), StageRestart());
}

void AFPGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(CountDownHandle, this, &AFPGameMode::CDTime, 1.0f, true, 1.0f);
}
