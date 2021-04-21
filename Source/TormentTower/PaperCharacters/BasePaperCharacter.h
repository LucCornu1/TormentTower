// Projet Bachelor Ludus Academie 2020-2021

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "BasePaperCharacter.generated.h"

class UPaperFlipbook;


UCLASS()
class TORMENTTOWER_API ABasePaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

private:
	// The animation to play while running around
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
		UPaperFlipbook* RunningAnimation;
	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
		UPaperFlipbook* IdleAnimation;
	// The animation to play when the character dies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
		UPaperFlipbook* DeathAnimation;
	// The animation to play when the character attacks
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
		UPaperFlipbook* AttackAnimation;
	// The animation to play when the character activate his special attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations", meta = (AllowPrivateAccess = "true"))
		UPaperFlipbook* SpecialAttackAnimation;


	FTimerHandle LoopTimerHandle;
	FTimerHandle AttackTimerHandle;

	// Death Animation Handler
	void OnAnimationEnd();

	// Knockback End
	void OnKnockbackEnd();

	// Attack Animation Handler
	void OnAttackEnd();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();
	void UpdateCharacter();

	// Character Stats
	bool bIsDead;
	bool bCanBeDamage;
	bool bIsAttacking;
	bool bCanBeKnockback;

	UPROPERTY(EditAnywhere, Category = "Character Stats")
		float MaxHP;
	float CurrentHP;

	UPROPERTY(EditAnywhere, Category = "Character Stats")
		float BruteForce;

	// Damage functions

	UFUNCTION()
		void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	UFUNCTION(BlueprintImplementableEvent)
		void OnHit_BP();

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

	// DeathHandle
	virtual void DeathHandle();

	// MoveFunction
	UFUNCTION(BlueprintCallable, Category = "ControlFunctions")
		virtual void MoveRight(float AxisValue);


public:
	// Sets default values for this character's properties
	ABasePaperCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	// Functions for HUD
	/** Assessor for the CurrentHP variable */
	UFUNCTION(BlueprintPure, Category = "HUD")
		float GetCurrentHP();

	/** Assessor for the bIsAttacking variable */
	UFUNCTION(BlueprintPure, Category = "Character Stats")
		bool GetIsAttacking();

	// AttackAnimationFunction
	UFUNCTION(BlueprintCallable, Category = "ControlFunctions")
		void Attack(bool bSpecialAttack = false);
};
