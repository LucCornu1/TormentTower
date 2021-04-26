// Projet Bachelor Ludus Academie 2020-2021


#include "PlayerPaperCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"


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
// End MoveRight functions


void APlayerPaperCharacter::DeathHandle()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ScreenDebug_DeathHandle_Message")));

	UCapsuleComponent* Capsule = this->GetCapsuleComponent();
	if (IsValid(Capsule))
	{
		Capsule->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

void APlayerPaperCharacter::PlayerAttack()
{
	Attack();
}