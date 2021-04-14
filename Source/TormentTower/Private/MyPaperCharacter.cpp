// Projet Bachelor Ludus Academie 2020-2021


#include "MyPaperCharacter.h"

// Sets default values
AMyPaperCharacter::AMyPaperCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MaxHealth = 10;

	/*StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMesh->SetupAttachment(RootComponent);

	PrimarySprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PrimarySprite"));
	PrimarySprite->SetupAttachment(RootComponent);*/

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1000.0f; // Longueur du bras. CE N'EST PAS LA DISTANCE DE VUE !!!!
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Create an orthographic camera (no perspective) and attach it to the boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	SideViewCameraComponent->OrthoWidth = 1600.0f; // Distance de vue entre la caméra et le joueur
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Configure character movement
	GravityScale = 2.0f;
	AirControl = 1.0f;
	JumpZVelocity = 1000.f;
	GroundFriction = 0.80f;
	MaxWalkSpeed = 800.0f;
	MaxFlySpeed = 1000.0f;

	GetCharacterMovement()->GravityScale = GravityScale;
	GetCharacterMovement()->AirControl = AirControl;
	GetCharacterMovement()->JumpZVelocity = JumpZVelocity;
	GetCharacterMovement()->GroundFriction = GroundFriction;
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	GetCharacterMovement()->MaxFlySpeed = MaxFlySpeed;

	// Configure character HUD variables
	CurrentHealth = MaxHealth;

	//Uniquement pour vérifier lorsque le moteur est déjà lancé, sinon il va crash au démarrage
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("%lld"), MaxHealth));
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("%lld"), CurrentHealth));*/

	bCanBeDamaged = true;
	bIsDead = false;
}

// Called when the game starts or when spawned
void AMyPaperCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPaperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCharacter();
}

// Called to bind functionality to input
void AMyPaperCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Les fonctions passées dans BindAxis peuvent prendre un argument
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyPaperCharacter::MoveRight);

	// Les fonctions passées dans BindAction ne doivent pas prendre d'arguments
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Damage", IE_Pressed, this, &AMyPaperCharacter::Damage);

	// Il ne faut pas énerver l'esprit de la machine
}

void AMyPaperCharacter::MoveRight(float AxisValue)
{
	// CurrentVelocity.X = FMath::Clamp(AxisValue, -1.0f, 1.0f) * 500.0f;

	// Bouge uniquement si le joueur n'est pas mort
	if (bIsDead)
	{
		return;
	}

	// Gestion de l'inertie
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue);
}

void AMyPaperCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Are we moving or standing still?
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}

}

void AMyPaperCharacter::UpdateCharacter()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, bIsDead ? TEXT("True") : TEXT("False"));
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

void AMyPaperCharacter::Damage()
{
	float FlipbookLengthInSeconds;

	if (bCanBeDamaged)
	{
		bCanBeDamaged = false;

		CurrentHealth -= 1;

		if (CurrentHealth <= 0)
		{
			CurrentHealth = 0;
			bIsDead = true;

			UPaperFlipbook* DesiredAnimation = DeathAnimation;

			GetSprite()->SetFlipbook(DesiredAnimation);

			FlipbookLengthInSeconds = GetSprite()->GetFlipbookLength();
			FlipbookLengthInSeconds -= 0.1f;

			GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &AMyPaperCharacter::OnAnimationEnd, FlipbookLengthInSeconds, false);

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Vous etes mort"));
		}
		else {
			GetWorld()->GetTimerManager().SetTimer(LoopTimerHandle, this, &AMyPaperCharacter::OnTimerEnd, 1.0f, false);
		}
	}
}

void AMyPaperCharacter::OnTimerEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%lld"), CurrentHealth));

	bCanBeDamaged = true;
}

void AMyPaperCharacter::OnAnimationEnd()
{
	GetSprite()->Stop();
}

int AMyPaperCharacter::GetCurrentHealth()
{
	return this->CurrentHealth;
}