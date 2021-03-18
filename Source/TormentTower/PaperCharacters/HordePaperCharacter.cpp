// Projet Bachelor Ludus Academie 2020-2021


#include "HordePaperCharacter.h"


// Sets default values
AHordePaperCharacter::AHordePaperCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHordePaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	ForwardAxisValue = GetActorForwardVector().X;

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), ForwardAxisValue));
}

// Called every frame
void AHordePaperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveRight(ForwardAxisValue);
}