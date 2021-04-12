// Projet Bachelor Ludus Academie 2020-2021


#include "HordePaperCharacter.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteActor.h"


// Sets default values
AHordePaperCharacter::AHordePaperCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanBeKnockback = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DANGER ZONE"));
	BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHordePaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	ForwardAxisValue = GetActorForwardVector().X;

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHordePaperCharacter::OnEnterDangerZone);
}

// Called every frame
void AHordePaperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveRight(ForwardAxisValue);
}

void AHordePaperCharacter::OnEnterDangerZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ABasePaperCharacter::StaticClass()) && !OtherActor->IsA(AHordePaperCharacter::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hello, Enter !!")));
	}

	if (OtherActor->IsA(APaperSpriteActor::StaticClass()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("DESTROY !!")));
	}
}