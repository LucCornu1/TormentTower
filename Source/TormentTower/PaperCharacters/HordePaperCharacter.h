// Projet Bachelor Ludus Academie 2020-2021

#pragma once

#include "CoreMinimal.h"
#include "BasePaperCharacter.h"
#include "HordePaperCharacter.generated.h"

class UBoxComponent;


UCLASS()
class TORMENTTOWER_API AHordePaperCharacter : public ABasePaperCharacter
{
	GENERATED_BODY()
	
private:
	// Range of the normal attack
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxComponent;
	// Range of the DeathZone
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* DeathZone_BoxComponent;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float ForwardAxisValue;

	// Called when the player Enter the DangerZone
	UFUNCTION()
		void OnEnterDangerZone(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Called when the player Enter the DeathZone
	UFUNCTION()
		void OnEnterDeathZone(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	// Sets default values for this character's properties
	AHordePaperCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
