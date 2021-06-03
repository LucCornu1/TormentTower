// Projet Bachelor Ludus Academie 2020-2021


#include "CustomPlayerController.h"
#include "../PaperCharacters/PlayerPaperCharacter.h"
#include "Kismet/GameplayStatics.h"

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void ACustomPlayerController::SetupInputComponent()
// BUT : Associer les contrôles clavier aux méthodes de déplacements des joueurs
// ENTREE : /
// SORTIE : /
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

void ACustomPlayerController::InitController(const bool bIsMultiplayer, APlayerPaperCharacter* J1, APlayerPaperCharacter* J2)
// BUT : Initialiser les pointeurs du joueur 1, et du second joueur si multijoueur
// ENTREE : Un booléen pour savoir si on est en multijoueur, le joueur 1 et le joueur 2
// SORTIE : /
{
	if (!IsValid(J1))
	{
		return;
	}

	Player1 = J1;

	if (bIsMultiplayer)
	{
		if (!IsValid(J2))
		{
			return;
		}

		Player2 = J2;
	}
}


// Appel des fonctions de déplacement & d'attaque des deux joueurs

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
		Player1->CharacterJump();
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
		Player2->CharacterJump();
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