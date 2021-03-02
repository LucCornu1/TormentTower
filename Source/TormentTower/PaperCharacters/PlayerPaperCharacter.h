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

	// MoveFunction
	UFUNCTION(BlueprintCallable, Category = "MoveFunction")
		void MoveRight(float AxisValue);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	APlayerPaperCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
