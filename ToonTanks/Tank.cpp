// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

//constructor
ATank::ATank()
{
    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArmComponent->SetupAttachment(RootComponent);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

    TankPlayerController = Cast<APlayerController>(GetController());	


}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
    PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (TankPlayerController) {
        FHitResult TraceHitResult;
        TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, TraceHitResult);
        FVector HitLocation = TraceHitResult.ImpactPoint;

        RotateTurret(HitLocation);

    }
}

void ATank::HandleDestruction() {
    Super::HandleDestruction();
    
    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
    bIsPlayerAlive = false;
}

void ATank::Move(float Value) {

    FVector DeltaLocation(0.f);

    DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);

	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value) {

    FRotator DeltaRotation(0.f);

    DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);

	AddActorLocalRotation(DeltaRotation, true);
}



