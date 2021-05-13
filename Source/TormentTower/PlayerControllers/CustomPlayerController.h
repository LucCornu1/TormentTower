// Projet Bachelor Ludus Academie 2020-2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

class APlayerPaperCharacter;


UCLASS()
class TORMENTTOWER_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	APlayerPaperCharacter* Player1;
	APlayerPaperCharacter* Player2;

	void MoveRightP1(float AxisValue);
	void MoveRightP2(float AxisValue);
	void JumpP1();
	void StopJumpP1();
	void JumpP2();
	void StopJumpP2();
	void AttackP1();
	void AttackP2();


protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Called to bind functionality to input
	void SetupInputComponent();


public:
	UFUNCTION(BlueprintCallable, Category = "Initialization")
		void InitController(bool bIsMultiplayer);


	FORCEINLINE
		APlayerPaperCharacter* GetPlayer1() { return Player1; };
	FORCEINLINE
		APlayerPaperCharacter* GetPlayer2() { return Player2; };

};
