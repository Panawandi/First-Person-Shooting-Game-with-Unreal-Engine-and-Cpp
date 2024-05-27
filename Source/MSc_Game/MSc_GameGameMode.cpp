// Copyright Epic Games, Inc. All Rights Reserved.

#include "MSc_GameGameMode.h"
#include "MSc_GameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMSc_GameGameMode::AMSc_GameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
