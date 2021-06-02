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
	// Si le personnage est mort, on n'exécute pas la fonction
	{
		return;
	}

	if (!bIsAttacking)
	// Si le personnage est en train d'attaquer, on ne mets pas à jour l'animation
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
// BUT : Infliger des dégâts au personnage quand un ennemi lui assène des coups
// ENTREE : L'acteur blessé, les dégâts subis, le type de dégâts (inutilisé), le responsable, et l'acteur ayant porté le coup
// SORTIE : Rien
{
	if (Damage <= 0 || bIsDead || !bCanBeDamage || !IsValid(DamageCauser))
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
		CharacterDeath();
	}
	else {
		// Si le personnage peut-être expulser, alors il l'est, sinon il ne se passe rien
		if (!IsValid(DamageCauser) || !bCanBeKnockback || DamageCauser->IsA(APaperSpriteActor::StaticClass()))
		{
			// Nothing
		}
		else {
			ABasePaperCharacter* Damager = Cast<ABasePaperCharacter>(DamageCauser);

			CharacterKnockback(Damager);
		}

		GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &ABasePaperCharacter::OnKnockbackEnd, 1.f, false);
	}
}

void ABasePaperCharacter::CharacterKnockback(ABasePaperCharacter* DamageCauser)
// BUT : Appliquer un "Knockback" au joueur (autrement dit un recul)
// ENTREE : L'acteur ayant porté le coup
// SORTIE : Rien
{
	if (IsValid(DamageCauser))
	{
		FVector VectorKnockbackDirection = DamageCauser->GetActorLocation() - this->GetActorLocation();
		GetCharacterMovement()->AirControl = 0.f;

		VectorKnockbackDirection.Normalize(0.f);

		FVector LaunchForce = -VectorKnockbackDirection * DamageCauser->BruteForce * 10.f;
		LaunchForce.Z = DamageCauser->BruteForce * 10;
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("%f"), Damager->BruteForce));
		LaunchCharacter(LaunchForce, true, true);
	}
}


void ABasePaperCharacter::OnAnimationEnd()
// BUT : Stopper l'animation du personnage à la fin de son animation de mort
{
	GetSprite()->Stop();
	DeathHandle();
}

void ABasePaperCharacter::OnKnockbackEnd()
// BUT : Rétablir les contrôles et retiré l'invincibilité au personnage frappé après un court délai
{
	GetCharacterMovement()->AirControl = AirControl;
	bCanBeDamage = true;
}

void ABasePaperCharacter::OnAttackEnd()
// BUT : Rétablir les contrôles au personnage attaquant avec un court délai
{
	bIsAttacking = false;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
}


void ABasePaperCharacter::MoveRight(float AxisValue)
{
	// Fonction vide, qui n'est pas virtuelle pure car Unreal ne le permet pas (voir mémoire)
}

void ABasePaperCharacter::Attack(const bool bSpecialAttack)
// BUT : Animer l'attaque du personnage
// ENTREE : Un booléen qui détermine si le coup porté est un coup spécial
// SORTIE : Rien
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

void ABasePaperCharacter::CharacterDeath()
// BUT : Animer la mort du personnage
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

void ABasePaperCharacter::DeathHandle()
// BUT : Détruire le personnage pour libérer de l'espace sur le terrain
{
	Destroy();
}

void ABasePaperCharacter::CancelGravity()
// BUT : Annuler l'effet de la gravité sur le personnage
{
	GetCharacterMovement()->GravityScale = 0.f;
}
