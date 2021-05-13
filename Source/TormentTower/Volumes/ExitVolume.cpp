// Projet Bachelor Ludus Academie 2020-2021


#include "ExitVolume.h"
#include "Components/BoxComponent.h"
#include "../PaperCharacters/PlayerPaperCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AExitVolume::AExitVolume()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("EXIT ZONE"));
	RootComponent = BoxComponent;
}

void AExitVolume::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AExitVolume::OnEnterExitZone);
}

void AExitVolume::OnEnterExitZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("EXIT !!")));

	if (IsValid(OtherActor) && OtherActor->IsA(APlayerPaperCharacter::StaticClass()))
	{
		APlayerPaperCharacter* Player = Cast<APlayerPaperCharacter>(OtherActor);
		Player->SetIsExited(true);
	}

	NextLevel();
}

void AExitVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExitVolume::NextLevel()
{
	int NbPlayersOut = 0;
	int NbPlayersDead = 0;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerPaperCharacter::StaticClass(), FoundActors);
	APlayerPaperCharacter* CurrentPlayer;
	int NbPlayers = FoundActors.Num();

	for (int i = 0; i < NbPlayers; i++)
	{
		CurrentPlayer = Cast<APlayerPaperCharacter>(FoundActors[i]);

		if (CurrentPlayer->GetIsExited())
		{
			NbPlayersOut++;
		}
		else if (CurrentPlayer->GetIsDead())
		{
			NbPlayersDead++;
		}
	}


	if ((NbPlayersOut + NbPlayersDead) == NbPlayers)
	{
		if (!NextLevelName.IsEmpty())
		{
			FName NLN = FName(NextLevelName);
			FText OutReason;

			if (NLN.IsValidObjectName(OutReason))
			{
				UGameplayStatics::OpenLevel(GetWorld(), NLN);
			}			
		}
		else {
			// Do something
		}
	}
}
