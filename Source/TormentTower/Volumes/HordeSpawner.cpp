// Projet Bachelor Ludus Academie 2020-2021


#include "HordeSpawner.h"
#include "Components/BoxComponent.h"
#include "../PaperCharacters/PlayerPaperCharacter.h"
#include "../PaperCharacters/HordePaperCharacter.h"


// Sets default values
AHordeSpawner::AHordeSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("SAFE ZONE"));

	static ConstructorHelpers::FObjectFinder<UBlueprint> PaperCharacterBlueprint(TEXT("Blueprint'/Game/Blueprints/PaperCharacters/BP_Horde.BP_Horde'"));
	if (PaperCharacterBlueprint.Object) {
		HordeBlueprint = (UClass*)PaperCharacterBlueprint.Object->GeneratedClass;
	}
}

// Called when the game starts or when spawned
void AHordeSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	// Box->OnComponentBeginOverlap.AddDynamic(this, &AHordeSpawner::OnEnterSafeZone);
	Box->OnComponentEndOverlap.AddDynamic(this, &AHordeSpawner::OnExitSafeZone);
}

// Called when the player enter the safe zone
/* void AHordeSpawner::OnEnterSafeZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerPaperCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hello, Enter !!")));
	}
} */

// Called when the player exit the safe zone
void AHordeSpawner::OnExitSafeZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerPaperCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hello, Exit !!")));

		FVector Location(645.f, 13.5f, -225.f);
		FRotator Rotation(0.f, 0.f, 0.f);
		FActorSpawnParameters SpawnInfo;
		GetWorld()->SpawnActor<AHordePaperCharacter>(HordeBlueprint, Location, Rotation, SpawnInfo);
	}
}

// Called every frame
void AHordeSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

