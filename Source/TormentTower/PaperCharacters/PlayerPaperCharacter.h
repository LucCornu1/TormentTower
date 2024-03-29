// Projet Bachelor Ludus Academie 2020-2021

#pragma once

#include "CoreMinimal.h"
#include "BasePaperCharacter.h"
#include "PlayerPaperCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ACustomPlayerController;


UCLASS()
class TORMENTTOWER_API APlayerPaperCharacter : public ABasePaperCharacter
{
	GENERATED_BODY()
	
private:
	// GameController
	UPROPERTY()
		ACustomPlayerController* CurrentController;

	// Number of the Player
	UPROPERTY(VisibleAnywhere, Category = "Player State")
		int PlayerNumber;
	// Boolean to state if the player has entered the ExitZone
	UPROPERTY(VisibleAnywhere, Category = "Player State")
		bool bIsExited;

	// Called when the player dies, to check if the game is over
	void CheckGameOver();


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


	// Player score inside the mini-games
	UPROPERTY(BlueprintReadWrite, Category = "mini-games")
		int PlayerScore;


	// Getters & Setters
	/** Mutator for the PlayerNumber variable */
	UFUNCTION(BlueprintCallable, Category = "Getters&Setters")
		void SetPlayerNumber(int Number) { PlayerNumber = Number; };

	/** Assessor for the PlayerNumber variable */
	UFUNCTION(BlueprintPure, Category = "Getters&Setters")
		int GetPlayerNumber() const { return PlayerNumber; };

	/** Mutator for the bIsExited variable */
	UFUNCTION(BlueprintCallable, Category = "Getters&Setters")
		void SetIsExited(bool b) { bIsExited = b; };

	/** Assessor for the bIsExited variable */
	UFUNCTION(BlueprintPure, Category = "Getters&Setters")
		bool GetIsExited() const { return bIsExited; };


	/** Controls functions */
	// PlayerAttack Function
	void PlayerAttack();

	// MoveFunction
	void MoveRight(float AxisValue) override;
	void CharacterJump();
};