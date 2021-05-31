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
	MoveRight(ForwardAxisValue);
}


bool AEnnemiPaperCharacter::DoLineTrace()
{
	FHitResult Hit;

	const float DetectionRange = 100.f;
	const FVector StartTrace = this->GetActorLocation();
	const FVector EndTrace = (this->GetActorForwardVector() * DetectionRange) + StartTrace;

	FCollisionQueryParams QueryParams = FCollisionQueryParams(SCENE_QUERY_STAT(Test), false, this);

	if (GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_Visibility, QueryParams))
	{
		/*if (ForwardAxisValue < 0.f)
		{
			this->SetActorRotation(FRotator(0.f, 0.f, 0.f));
		}
		else if (ForwardAxisValue > 0.f)
		{
			this->SetActorRotation(FRotator(0.f, 180.f, 0.f));
		}*/

		// DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), false);

		return true;
	}

	return false;
}

void AEnnemiPaperCharacter::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
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