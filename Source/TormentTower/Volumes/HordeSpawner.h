// Projet Bachelor Ludus Academie 2020-2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HordeSpawner.generated.h"

class UBoxComponent;
class AHordePaperCharacter;

UCLASS()
class TORMENTTOWER_API AHordeSpawner : public AActor
{
	GENERATED_BODY()
	
private:
	// Safe Zone, when the player Exit it, the Horde spawns
	UPROPERTY()
		UBoxComponent* Box;

	TSubclassOf<class AHordePaperCharacter> HordeBlueprint;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the player Enter the SafeZone
	/* UFUNCTION()
		void OnEnterSafeZone(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	*/

	// Called when the player Exit the SafeZone
	UFUNCTION()
		void OnExitSafeZone(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Sets default values for this actor's properties
	AHordeSpawner();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
