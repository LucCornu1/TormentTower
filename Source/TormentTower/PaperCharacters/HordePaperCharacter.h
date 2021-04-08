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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float ForwardAxisValue;


public:
	// Sets default values for this character's properties
	AHordePaperCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
