// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "FPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MSC_GAME_API AFPGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	void GameReset(bool Victory);

public:
	UPROPERTY(BlueprintReadOnly)
		int CountDown = 120;

public:
	void BeginPlay() override;

private:
	void StageRestart();

private:
	FTimerHandle CountDownHandle = FTimerHandle();

	void CDTime();
};
