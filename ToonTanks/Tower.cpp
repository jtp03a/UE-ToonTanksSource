// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Find the distance to Tank
    if (TankInRange()) {
        RotateTurret(Tank->GetActorLocation());   
    }
   

}

void ATower::HandleDestruction() {
    Super::HandleDestruction();
    Destroy();
}

void ATower::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::CheckFireCondition() {
    if (TankInRange()) {
        Fire();
    }
}

bool ATower::TankInRange() {
    if (Tank) {
        float Distance = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());
        return (Distance <= FireRange);
    }
    
    return false;
}   
