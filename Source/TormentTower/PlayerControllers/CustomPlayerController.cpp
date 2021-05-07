// Projet Bachelor Ludus Academie 2020-2021


#include "CustomPlayerController.h"
#include "../PaperCharacters/PlayerPaperCharacter.h"
#include "Kismet/GameplayStatics.h"

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ACustomPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveRightP1", this, &ACustomPlayerController::MoveRightP1);
	InputComponent->BindAxis("MoveRightP2", this, &ACustomPlayerController::MoveRightP2);

	InputComponent->BindAction("JumpP1", IE_Pressed, this, &ACustomPlayerController::JumpP1);
	InputComponent->BindAction("JumpP1", IE_Released, this, &ACustomPlayerController::StopJumpP1);
	InputComponent->BindAction("JumpP2", IE_Pressed, this, &ACustomPlayerController::JumpP2);
	InputComponent->BindAction("JumpP2", IE_Released, this, &ACustomPlayerController::StopJumpP2);
	InputComponent->BindAction("AttackP1", IE_Pressed, this, &ACustomPlayerController::AttackP1);
	InputComponent->BindAction("AttackP2", IE_Pressed, this, &ACustomPlayerController::AttackP2);
}

void ACustomPlayerController::InitController(bool bIsMultiplayer)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerPaperCharacter::StaticClass(), FoundActors);

	Player1 = Cast<APlayerPaperCharacter>(FoundActors[0]);

	if (bIsMultiplayer)
	{
		Player2 = Cast<APlayerPaperCharacter>(FoundActors[1]);
	}
}


void ACustomPlayerController::MoveRightP1(float AxisValue)
{
	if (IsValid(Player1))
	{
		Player1->MoveRight(AxisValue);
	}
}

void ACustomPlayerController::MoveRightP2(float AxisValue)
{
	if (IsValid(Player2))
	{
		Player2->MoveRight(AxisValue);
	}
}

void ACustomPlayerController::JumpP1()
{
	if (IsValid(Player1))
	{
		Player1->Jump();
	}
}

void ACustomPlayerController::StopJumpP1()
{
	if (IsValid(Player1))
	{
		Player1->StopJumping();
	}
}

void ACustomPlayerController::JumpP2()
{
	if (IsValid(Player2))
	{
		Player2->Jump();
	}
}

void ACustomPlayerController::StopJumpP2()
{
	if (IsValid(Player2))
	{
		Player2->StopJumping();
	}
}

void ACustomPlayerController::AttackP1()
{
	if (IsValid(Player1))
	{
		Player1->PlayerAttack();
	}
}

void ACustomPlayerController::AttackP2()
{
	if (IsValid(Player2))
	{
		Player2->PlayerAttack();
	}
}