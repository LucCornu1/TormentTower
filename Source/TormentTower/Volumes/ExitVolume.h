// Projet Bachelor Ludus Academie 2020-2021

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "ExitVolume.generated.h"

class UBoxComponent;


UCLASS()
class TORMENTTOWER_API AExitVolume : public APaperSpriteActor
{
	GENERATED_BODY()
	
private:
	// Safe Zone, when the player Exit it, the Horde spawns
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;

	// Next Level Name
	/** The name of the next level */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Misc", meta = (AllowPrivateAccess = "true"))
		FString NextLevelName;

	TArray<AActor*> FoundActors;


protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

	// Called when the player Enter the ExitZone
	UFUNCTION(BlueprintCallable, Category = "Misc")
		void OnEnterExitZone(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	// Sets default values for this actor's properties
	AExitVolume();

	// Called every frame
	void Tick(float DeltaTime) override;

	// Next level function
	void NextLevel();
};
