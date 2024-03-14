// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameModeBase.h"
#include "FPSPlayerController.h"

void AMyGameModeBase::StartPlay()
{
    Super::StartPlay();
}

void AMyGameModeBase::HandleGameOver()
{
    AFPSPlayerController* Controller = Cast<AFPSPlayerController>(GetWorld()->GetFirstPlayerController());
    Controller->ShowGameOverUI();
}