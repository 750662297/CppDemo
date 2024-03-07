// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AFPSPlayerController::BeginPlay()
{
    Super::BeginPlay();

    //获取输入映射子系统
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);

        UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
    }
}

