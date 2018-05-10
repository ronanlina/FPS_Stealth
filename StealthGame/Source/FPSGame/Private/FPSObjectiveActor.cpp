// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSObjectiveActor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/GameplayStatics.h" //used in PlayEffects()
#include "FPSCharacter.h"//used when character picked up the pickup actor and identify as carrying the objective

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //Mesh Component (root) should have no collision
	RootComponent = MeshComp; //first in the heirarchy
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp); // attaches to MeshComp's heirarchy
}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSObjectiveActor::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation()); //which world, Emitter template, location
}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

		PlayEffects();

		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor); // casting the overlapping actor to MyCharacter variable.
		
				//check if overlapped with AFPSCharacter type	
			if (MyCharacter)
			{
				MyCharacter->bIsCarryingObjective = true; //current MyCharacter actor is carrying the objective
			
					Destroy();
			}
}

