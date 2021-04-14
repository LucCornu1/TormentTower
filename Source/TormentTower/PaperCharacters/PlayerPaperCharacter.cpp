// Projet Bachelor Ludus Academie 2020-2021


#include "PlayerPaperCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
APlayerPaperCharacter::APlayerPaperCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Default Components value
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 1000.0f; // Longueur du bras
	// CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	CameraComponent->OrthoWidth = 2000.0f; // Distance de vue entre la caméra et le joueur
	CameraComponent->SetupAttachment(CameraBoom);
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
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerPaperCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerPaperCharacter::PlayerAttack);
}


void APlayerPaperCharacter::MoveRight(float AxisValue)
{
	if (bIsDead)
	{
		return;
	}

	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), AxisValue);
}


void APlayerPaperCharacter::DeathHandle()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("ScreenDebug_DeathHandle_Message")));
}


void APlayerPaperCharacter::PlayerAttack()
{
	Attack();
}