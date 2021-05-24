// Projet Bachelor Ludus Academie 2020-2021


#include "HordePaperCharacter.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteActor.h"
#include "EnnemiPaperCharacter.h"
#include "PlayerPaperCharacter.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHordePaperCharacter::AHordePaperCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bCanBeKnockback = false;
	bCanDamage = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DANGER ZONE"));
	BoxComponent->SetupAttachment(RootComponent);

	DeathZone_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DEATH ZONE"));
	DeathZone_BoxComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHordePaperCharacter::BeginPlay()
{
	Super::BeginPlay();

	ForwardAxisValue = GetActorForwardVector().X;

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHordePaperCharacter::OnEnterDangerZone);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AHordePaperCharacter::OnExitDangerZone);
	DeathZone_BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHordePaperCharacter::OnEnterDeathZone);
}

// Called every frame
void AHordePaperCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveRight(ForwardAxisValue);
	DamageCharacter();
}

void AHordePaperCharacter::OnEnterDangerZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetName() == TEXT("AttackHitbox") || !IsValid(OtherActor))
	{
		return;
	}

	if (OtherActor->IsA(ABasePaperCharacter::StaticClass()) && !OtherActor->IsA(AHordePaperCharacter::StaticClass()))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hello, Enter !!")));

		if (OtherActor->IsA(AEnnemiPaperCharacter::StaticClass()))
		{
			OtherActor->Destroy();
		}
		else {
			bCanDamage = true;
			Target = Cast<APlayerPaperCharacter>(OtherActor);
		}
	}
	else if (OtherActor->IsA(APaperSpriteActor::StaticClass()))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("DESTROY !!")));

		TArray<AActor*> AttachedActors;
		OtherActor->GetAttachedActors(AttachedActors, true);

		for (int i = 0; i < AttachedActors.Num(); i++)
		{
			AttachedActors[i]->Destroy();
		}

		OtherActor->Destroy();
	}

	Attack();
}

void AHordePaperCharacter::OnExitDangerZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetName() == TEXT("AttackHitbox") || !IsValid(OtherActor))
	{
		return;
	}

	if (OtherActor->IsA(ABasePaperCharacter::StaticClass()) && !OtherActor->IsA(AHordePaperCharacter::StaticClass()))
	{
		bCanDamage = false;
	}
}

void AHordePaperCharacter::OnEnterDeathZone(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetName() == TEXT("AttackHitbox"))
	{
		return;
	}

	if (OtherActor->IsA(ABasePaperCharacter::StaticClass()) && !OtherActor->IsA(AHordePaperCharacter::StaticClass()))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hello, Enter !!")));

		TSubclassOf<UDamageType> P;
		UGameplayStatics::ApplyDamage(OtherActor, 4.f, GetController(), this, P);
	}

	Attack(true);
}

void AHordePaperCharacter::DamageCharacter()
{
	if (IsValid(Target) && bCanDamage)
	{
		TSubclassOf<UDamageType> P;
		UGameplayStatics::ApplyDamage(Target, 1.f, GetController(), this, P);
	}
}