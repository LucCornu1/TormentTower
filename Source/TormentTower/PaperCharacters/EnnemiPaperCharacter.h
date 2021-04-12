// Projet Bachelor Ludus Academie 2020-2021

#pragma once

#include "CoreMinimal.h"
#include "BasePaperCharacter.h"
#include "EnnemiPaperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TORMENTTOWER_API AEnnemiPaperCharacter : public ABasePaperCharacter
{
	GENERATED_BODY()
	
private:
	bool bWallDetected;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool DoLineTrace();

	float ForwardAxisValue;


public:
	// Sets default values for this character's properties
	AEnnemiPaperCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
