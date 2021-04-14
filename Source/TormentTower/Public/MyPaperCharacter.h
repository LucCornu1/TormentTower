// Projet Bachelor Ludus Academie 2020-2021

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperCharacter.h"
#include "Components/TimelineComponent.h"
#include "Components/BoxComponent.h"
#include "MyPaperCharacter.generated.h"

/**
 *
 */
UCLASS()
class TORMENTTOWER_API AMyPaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyPaperCharacter();

private:
	// Functions
	void MoveRight(float AxisValue);

	FTimerHandle LoopTimerHandle;

	void OnTimerEnd();

	void OnAnimationEnd();

	/*// Variables
	FVector CurrentVelocity;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, Category = "Components")
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Sprites")
		class UPaperSpriteComponent* PrimarySprite;

	// The animation to play while running around
	UPROPERTY(EditAnywhere, Category = "Animations")
		class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, Category = "Animations")
		class UPaperFlipbook* IdleAnimation;

	// The animation to play when the character dies
	UPROPERTY(EditAnywhere, Category = "Animations")
		class UPaperFlipbook* DeathAnimation;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();

	void UpdateCharacter();


	// HUD variables
	/** Variable for the max health of the character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		int CurrentHealth;

	bool bCanBeDamaged, bIsDead;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// Controls variables
	UPROPERTY(EditAnywhere, Category = "Controls")
		float GravityScale;

	UPROPERTY(EditAnywhere, Category = "Controls")
		float AirControl;

	UPROPERTY(EditAnywhere, Category = "Controls")
		float JumpZVelocity;

	UPROPERTY(EditAnywhere, Category = "Controls")
		float GroundFriction;

	UPROPERTY(EditAnywhere, Category = "Controls")
		float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, Category = "Controls")
		float MaxFlySpeed;


	// Damage function
	void Damage();

	// Functions for HUD
	/** Assessor for the CurrentHealth variable */
	UFUNCTION(BlueprintPure, Category = "HUD")
		int GetCurrentHealth();
};
