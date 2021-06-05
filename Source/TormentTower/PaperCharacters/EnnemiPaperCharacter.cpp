// Projet Bachelor Ludus Academie 2020-2021


#include "EnnemiPaperCharacter.h"
#include "Engine.h"
#include "PlayerPaperCharacter.h"


// Sets default values
AEnnemiPaperCharacter::AEnnemiPaperCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnnemiPaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	ForwardAxisValue = GetActorForwardVector().X;
	bWallDetected = false;

	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), ForwardAxisValue));
}

// Called every frame
void AEnnemiPaperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*
	bWallDetected = DoLineTrace();

	if (bWallDetected)
	{
		if (ForwardAxisValue < 0.f)
		{
			this->SetActorRotation(FRotator(0.f, 0.f, 0.f));
		}
		else if (ForwardAxisValue > 0.f)
		{
			this->SetActorRotation(FRotator(0.f, 180.f, 0.f));
		}
	}

	ForwardAxisValue = GetActorForwardVector().X;
	MoveRight(ForwardAxisValue);*/
}


bool AEnnemiPaperCharacter::DoLineTrace()
// BUT : Tirer un trait en face du personnage, qui va détecter la présence d'un mur
// ENTREE : Rien
//SORTIE : Vrai si un mur est détecté, faux dans le cas contraire
{
	FHitResult Hit;

	const float DetectionRange = 100.f;
	const FVector StartTrace = this->GetActorLocation();
	const FVector EndTrace = (this->GetActorForwardVector() * DetectionRange) + StartTrace;

	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(Test), false, this);

	return GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams);
}

void AEnnemiPaperCharacter::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
// BUT : Redéfinition de la méthode TakeDamage, pour attribuer des points de score si un joueur abbat cet ennemi
// ENTREE : L'acteur blessé, les dégâts subis, le type de dégâts (inutilisé), le responsable, et l'acteur ayant porté le coup
// SORTIE : Rien
{
	if (!IsValid(DamageCauser))
	{
		return;
	}

	Super::TakeDamage(DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);
	
	if (DamageCauser->IsA(APlayerPaperCharacter::StaticClass()) && bIsDead)
	{
		APlayerPaperCharacter* Damager = Cast<APlayerPaperCharacter>(DamageCauser);

		Damager->PlayerScore += 25;
	}
}

void AEnnemiPaperCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue);
}