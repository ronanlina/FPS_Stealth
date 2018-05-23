// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "FPSGameMode.h"
// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12.f, FColor::Green, false, 10.f);

	UE_LOG(LogTemp, Warning, TEXT("Overlapped"))
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());// Getting the game mode
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}

}

void AFPSAIGuard::OnNoiseHeard(APawn * NoiseInstigator, const FVector & Location, float Volume)
{
	DrawDebugSphere(GetWorld(), Location, 32.f, 12.f, FColor::Blue, false, 10.f);
	
	//Distance from where the sound was made subtracted to the actor location
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

						//direction vector
	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.f;
	NewLookAt.Roll = 0.f;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);

	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);
}

void AFPSAIGuard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	OriginalRotation = GetActorRotation();
	
}
// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
// Called to bind functionality to input
void AFPSAIGuard::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

