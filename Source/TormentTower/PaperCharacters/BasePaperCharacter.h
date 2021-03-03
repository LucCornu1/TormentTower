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


	FTimerHandle LoopTimerHandle;

	// Death Animation Handler
	void OnAnimationEnd();

	// Knockback End
	void OnKnockbackEnd();



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Called to choose the correct animation to play based on the character's movement state */
	void UpdateAnimation();
	void UpdateCharacter();

	// Character Stats
	bool bIsDead;
	float MaxHP;
	float CurrentHP;

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


public:
	// Sets default values for this character's properties
	ABasePaperCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	// Functions for HUD
	/** Assessor for the CurrentHP variable */
	UFUNCTION(BlueprintPure, Category = "HUD")
		float GetCurrentHP();
};
