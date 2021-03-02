// Projet Bachelor Ludus Academie 2020-2021


#include "BasePaperCharacter.h"
#include "PaperFlipbookComponent.h"
// #include "Components/TimelineComponent.h"


// Sets default values
ABasePaperCharacter::ABasePaperCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Default stats value
	bIsDead = false;
}

// Called when the game starts or when spawned
void ABasePaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->OnTakeAnyDamage.AddDynamic(this, &ABasePaperCharacter::TakeDamage);
}

// Called every frame
void ABasePaperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCharacter();
}


void ABasePaperCharacter::UpdateCharacter()
{
	if (bIsDead)
	{
		return;
	}

	// Update animation to match the motion
	UpdateAnimation();

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
		}
	}
}

void ABasePaperCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = AActor::GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}

}


void ABasePaperCharacter::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	CurrentHP = FMath::Clamp(CurrentHP - Damage, 0.f, MaxHP);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHP));

	OnHit_BP();

	if (CurrentHP <= 0.f)
	{
		float FlipbookLengthInSeconds;

		bIsDead = true;

		UPaperFlipbook* DesiredAnimation = DeathAnimation;
		GetSprite()->SetFlipbook(DesiredAnimation);
		FlipbookLengthInSeconds = GetSprite()->GetFlipbookLength();
		FlipbookLengthInSeconds -= 0.1f;

		GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &ABasePaperCharacter::OnAnimationEnd, FlipbookLengthInSeconds, false);
	}
}

void ABasePaperCharacter::OnAnimationEnd()
{
	GetSprite()->Stop();
}


float ABasePaperCharacter::GetCurrentHP()
{
	return this->CurrentHP;
}