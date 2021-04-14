// Copyright Epic Games, Inc. All Rights Reserved.

#include "TormentTowerGameMode.h"
#include "PaperCharacters/PlayerPaperCharacter.h"
#include "MyCharacterHUD.h"
#include "Kismet/GameplayStatics.h"

ATormentTowerGameMode::ATormentTowerGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = APlayerPaperCharacter::StaticClass();

	HUDClass = AMyCharacterHUD::StaticClass();
}

void ATormentTowerGameMode::BeginPlay()
{
	Super::BeginPlay();
}
