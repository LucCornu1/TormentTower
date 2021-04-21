// Projet Bachelor Ludus Academie 2020-2021

#pragma once

#include "CoreMinimal.h"
#include "BasePaperCharacter.h"
#include "PlayerPaperCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;


UCLASS()
class TORMENTTOWER_API APlayerPaperCharacter : public ABasePaperCharacter
{
	GENERATED_BODY()
	
private:
	/** Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* CameraComponent;
	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;

	// Number of the Player
	UPROPERTY(VisibleAnywhere, Category = "Player Number")
		int PlayerNumber;


protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// DeathHandle Override
	void DeathHandle() override;


public:
	// Sets default values for this character's properties
	APlayerPaperCharacter();

	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Setter for PlayerNumber
	UFUNCTION(BlueprintCallable, Category = "Setter")
		void SetPlayerNumber(int Number);

	// Getter fot PlayerNumber
	UFUNCTION(BlueprintPure, Category = "Getter")
		int GetPlayerNumber();


	/** Controls functions */

	// PlayerAttack Function
	void PlayerAttack();

	// MoveFunction
	void MoveRight(float AxisValue) override;
};
