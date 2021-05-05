// Projet Bachelor Ludus Academie 2020-2021


#include "BasePaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerPaperCharacter.h"
#include "PaperSpriteActor.h"


// Sets default values
ABasePaperCharacter::ABasePaperCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Default stats value
	bIsDead = false;
	bCanBeDamage = true;
	bIsAttacking = false;
	bCanBeKnockback = true;

	MaxHP = 4.f;
	BruteForce = 0.f;


	// Configure character movement
	GravityScale = 2.0f;
	AirControl = 1.0f;
	JumpZVelocity = 1000.f;
	GroundFriction = 0.80f;
	MaxWalkSpeed = 600.0f;
	MaxFlySpeed = 1000.0f;

	GetCharacterMovement()->GravityScale = GravityScale;
	GetCharacterMovement()->AirControl = AirControl;
	GetCharacterMovement()->JumpZVelocity = JumpZVelocity;
	GetCharacterMovement()->GroundFriction = GroundFriction;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MaxFlySpeed = MaxFlySpeed;
}

// Called when the game starts or when spawned
void ABasePaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = MaxHP;

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

	if (!bIsAttacking)
	{
		// Update animation to match the motion
		UpdateAnimation();
	}

	// Now setup the rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;
	// Set the rotation so that the character faces his direction of travel.
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.f)
		{
			Controller->SetControlRotation(FRotator(0.f, 180.f, 0.f));
		}
		else if (TravelDirection > 0.f)
		{
			Controller->SetControlRotation(FRotator(0.f, 0.f, 0.f));
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
	if (Damage <= 0 || bIsDead || !bCanBeDamage)
	{
		return;
	}

	bCanBeDamage = false;

	if (DamagedActor->IsA(APlayerPaperCharacter::StaticClass()) && DamageCauser->IsA(APlayerPaperCharacter::StaticClass()))
	{
		// Nothing
	}
	else {
		CurrentHP = FMath::Clamp(CurrentHP - Damage, 0.f, MaxHP);
		OnHit_BP();
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHP));

	if (CurrentHP <= 0.f)
	{
		bIsDead = true;
		float FlipbookLengthInSeconds;

		UPaperFlipbook* DesiredAnimation = DeathAnimation;
		if (DesiredAnimation != nullptr)
		{
			GetSprite()->SetFlipbook(DesiredAnimation);
			FlipbookLengthInSeconds = GetSprite()->GetFlipbookLength();
			FlipbookLengthInSeconds -= 0.1f;

			GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &ABasePaperCharacter::OnAnimationEnd, FlipbookLengthInSeconds, false);
		}
	}
	else {
		if (!IsValid(DamageCauser) || !bCanBeKnockback || DamageCauser->IsA(APaperSpriteActor::StaticClass()))
		{
			// Nothing
		}
		else {
			ABasePaperCharacter* Damager = dynamic_cast<ABasePaperCharacter*>(DamageCauser);

			if (IsValid(Damager))
			{
				FVector VectorKnockbackDirection = Damager->GetActorLocation() - this->GetActorLocation();
				GetCharacterMovement()->AirControl = 0.f;

				VectorKnockbackDirection.Normalize(0.f);

				FVector LaunchForce = -VectorKnockbackDirection * Damager->BruteForce * 10.f;
				LaunchForce.Z = Damager->BruteForce * 10;
				// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("%f"), Damager->BruteForce));
				LaunchCharacter(LaunchForce, true, true);
			}
		}

		GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &ABasePaperCharacter::OnKnockbackEnd, 1.f, false);
	}
}


void ABasePaperCharacter::OnAnimationEnd()
{
	GetSprite()->Stop();
	DeathHandle();
}

void ABasePaperCharacter::OnKnockbackEnd()
{
	GetCharacterMovement()->AirControl = AirControl;
	bCanBeDamage = true;
}

void ABasePaperCharacter::OnAttackEnd()
{
	bIsAttacking = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}


void ABasePaperCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue);
}

void ABasePaperCharacter::Attack(bool bSpecialAttack)
{
	if (bIsAttacking)
	{
		return;
	}

	bIsAttacking = true;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;

	float FlipbookLengthInSeconds;

	if (!bSpecialAttack)
	{
		GetSprite()->SetFlipbook(AttackAnimation);
	}
	else {
		GetSprite()->SetFlipbook(SpecialAttackAnimation);
	}
	

	FlipbookLengthInSeconds = GetSprite()->GetFlipbookLength();
	FlipbookLengthInSeconds -= 0.1f;

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ABasePaperCharacter::OnAttackEnd, FlipbookLengthInSeconds, false);
}

void ABasePaperCharacter::DeathHandle()
{
	Destroy();
}