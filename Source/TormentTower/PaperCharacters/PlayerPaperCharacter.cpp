// Projet Bachelor Ludus Academie 2020-2021


#include "PlayerPaperCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../PlayerControllers/CustomPlayerController.h"


// Sets default values
APlayerPaperCharacter::APlayerPaperCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bIsExited = false;
}

// Called when the game starts or when spawned
void APlayerPaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerScore = 0;
}

void APlayerPaperCharacter::CheckGameOver()
{
	CurrentController = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerNumber == 0)
	{
		if (IsValid(CurrentController->GetPlayer2()))
		{
			if (CurrentController->GetPlayer2()->GetIsDead())
			{
				UGameplayStatics::OpenLevel(GetWorld(), "Menu");
			}
		}
		else {
			UGameplayStatics::OpenLevel(GetWorld(), "Menu");
		}
	}
	else
	{
		if (CurrentController->GetPlayer1()->GetIsDead())
		{
			UGameplayStatics::OpenLevel(GetWorld(), "Menu");
		}
	}

	
}

// Called every frame
void APlayerPaperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPaperCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// MoveRight functions 
void APlayerPaperCharacter::MoveRight(float AxisValue)
{
	if (!bIsDead)
	{
		this->AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue);
	}
}

void APlayerPaperCharacter::CharacterJump()
{
	if (!bIsDead)
	{
		Jump();
	}
}
// End MoveRight functions


void APlayerPaperCharacter::DeathHandle()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ScreenDebug_DeathHandle_Message")));

	UCapsuleComponent* Capsule = this->GetCapsuleComponent();
	if (IsValid(Capsule))
	{
		Capsule->SetCollisionProfileName(TEXT("NoCollision"));

		CancelGravity();
	}
	CheckGameOver();
}

void APlayerPaperCharacter::PlayerAttack()
{
	Attack();
}