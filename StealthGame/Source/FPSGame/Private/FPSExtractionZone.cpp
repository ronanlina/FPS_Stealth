// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/Gameplaystatics.h" // to play sound

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0f));
	RootComponent = OverlapComp;

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f);
	DecalComp->SetupAttachment(RootComponent);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

		AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);

		if (MyPawn == nullptr)
		{		
			return;
		}

		if (MyPawn->bIsCarryingObjective)
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapped"))
				AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());// Getting the game mode
			if (GM)
			{
				GM->CompleteMission(MyPawn);
			}
		}
		else 
		{
			UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound); //Responsible for sound played when pawn colliding with isCarryingObjective equal to false
		}
}
