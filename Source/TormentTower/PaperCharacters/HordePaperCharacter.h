// Projet Bachelor Ludus Academie 2020-2021

#pragma once

#include "CoreMinimal.h"
#include "BasePaperCharacter.h"
#include "HordePaperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TORMENTTOWER_API AHordePaperCharacter : public ABasePaperCharacter
{
	GENERATED_BODY()
	
private:
	// MoveFunction
	UFUNCTION(BlueprintCallable, Category = "MoveFunction")
		void MoveRight(float AxisValue);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Sets default values for this character's properties
	AHordePaperCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
