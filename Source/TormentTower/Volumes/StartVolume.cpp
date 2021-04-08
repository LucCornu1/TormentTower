// Projet Bachelor Ludus Academie 2020-2021


#include "StartVolume.h"
#include "Components/BoxComponent.h"
#include "../PaperCharacters/PlayerPaperCharacter.h"
#include "../PaperCharacters/HordePaperCharacter.h"


// Sets default values
AStartVolume::AStartVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SAFE ZONE"));
	RootComponent = BoxComponent;

	static ConstructorHelpers::FObjectFinder<UBlueprint> PaperCharacterBlueprint(TEXT("Blueprint'/Game/Blueprints/PaperCharacters/BP_Horde.BP_Horde'"));
	if (PaperCharacterBlueprint.Object) {
		HordeBlueprint = (UClass*)PaperCharacterBlueprint.Object->GeneratedClass;
	}

	bHordeSpawned = false;
}

// Called when the game starts or when spawned
void AStartVolume::BeginPlay()
{
	Super::BeginPlay();

	// BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHordeSpawner::OnEnterSafeZone);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AStartVolume::OnExitSafeZone);
}

// Called when the player enter the safe zone
/* void AStartVolume::OnEnterSafeZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerPaperCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hello, Enter !!")));
	}
} */

// Called when the player exit the safe zone
void AStartVolume::OnExitSafeZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerPaperCharacter::StaticClass()))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hello, Exit !!")));

		if (!bHordeSpawned)
		{
			bHordeSpawned = true;

			FVector Location(645.f, 13.5f, -200.f);
			FRotator Rotation(0.f, 0.f, 0.f);
			FActorSpawnParameters SpawnInfo;
			GetWorld()->SpawnActor<AHordePaperCharacter>(HordeBlueprint, Location, Rotation, SpawnInfo);
		}
	}
}

// Called every frame
void AStartVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
