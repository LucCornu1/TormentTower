// Projet Bachelor Ludus Academie 2020-2021


#include "BasePaperCharacter.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABasePaperCharacter::ABasePaperCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Default stats value
	bIsDead = false;
	bCanBeDamage = true;
	bIsAttacking = false;

	MaxHP = 4.f;
	CurrentHP = MaxHP;


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
	if (Damage <= 0 || bIsDead || !bCanBeDamage)
	{
		return;
	}

	bCanBeDamage = false;
	CurrentHP = FMath::Clamp(CurrentHP - Damage, 0.f, MaxHP);
	OnHit_BP();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHP));

	if (CurrentHP <= 0.f)
	{
		bIsDead = true;
		float FlipbookLengthInSeconds;

		UPaperFlipbook* DesiredAnimation = DeathAnimation;
		GetSprite()->SetFlipbook(DesiredAnimation);
		FlipbookLengthInSeconds = GetSprite()->GetFlipbookLength();
		FlipbookLengthInSeconds -= 0.1f;

		GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &ABasePaperCharacter::OnAnimationEnd, FlipbookLengthInSeconds, false);
	}
	else {
		if (!DamageCauser)
		{
			// Nothing
		}
		else {
			FVector VectorKnockbackDirection = DamageCauser->GetActorLocation() - this->GetActorLocation();
			GetCharacterMovement()->AirControl = 0.f;

			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("%f"), VectorKnockbackDirection.X));
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, FString::Printf(TEXT("%f"), VectorKnockbackDirection.Y));
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Silver, FString::Printf(TEXT("%f"), VectorKnockbackDirection.Z));

			VectorKnockbackDirection.Normalize(0.f);

			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%f"), VectorKnockbackDirection.X));
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("%f"), VectorKnockbackDirection.Y));
			// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%f"), VectorKnockbackDirection.Z));

			FVector LaunchForce = -VectorKnockbackDirection * 900.f;
			LaunchCharacter(LaunchForce, true, true);
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


float ABasePaperCharacter::GetCurrentHP()
{
	return this->CurrentHP;
}

bool ABasePaperCharacter::GetIsAttacking()
{
	return bIsAttacking;
}


void ABasePaperCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue);
}

void ABasePaperCharacter::Attack()
{
	if (bIsAttacking)
	{
		return;
	}

	bIsAttacking = true;
	GetCharacterMovement()->MaxWalkSpeed = 0.f;

	float FlipbookLengthInSeconds;

	GetSprite()->SetFlipbook(AttackAnimation);

	FlipbookLengthInSeconds = GetSprite()->GetFlipbookLength();
	FlipbookLengthInSeconds -= 0.1f;

	GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &ABasePaperCharacter::OnAttackEnd, FlipbookLengthInSeconds, false);
}

void ABasePaperCharacter::DeathHandle()
{
	Destroy();
}