// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTankGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h" 
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTankGameMode::BeginPlay() {
    Super::BeginPlay();

    HandleGameStart();

}

void AToonTankGameMode::ActorDied(AActor* DeadActor) {
    if (DeadActor == Tank) {
        Tank->HandleDestruction();
        if (ToonTanksPlayerController) {
            ToonTanksPlayerController->SetPlayerEnabledState(false);
        }
        GameOver(false);
    } else if (ATower* DestroyedTower = Cast<ATower>(DeadActor)) {
            DestroyedTower->HandleDestruction();
            TargetTowers -= 1;
            if (TargetTowers == 0) {
                GameOver(true);
            }
    }
}

void AToonTankGameMode::HandleGameStart() {
    TargetTowers = GetTargetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (ToonTanksPlayerController) {
        ToonTanksPlayerController->SetPlayerEnabledState(false);

        FTimerHandle PlayerEnableHandle;
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(ToonTanksPlayerController, &AToonTanksPlayerController::SetPlayerEnabledState, true);
        GetWorldTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }
}

int32 AToonTankGameMode::GetTargetTowerCount() {
    TArray<AActor*> Towers;
    
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    
    return Towers.Num();
}

