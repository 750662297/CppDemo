// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AFPSPlayerController::AFPSPlayerController()
{
    static ConstructorHelpers::FClassFinder<UUserWidget>WidgetClass(TEXT("WidgetBlueprint'/Game/UI/UMG/UMG_MainMenu.UMG_MainMenu_C'"));
    if (WidgetClass.Succeeded())
    {
        MenuWidgetClass = WidgetClass.Class;
    }
}

void AFPSPlayerController::BeginPlay()
{
    Super::BeginPlay();

    //获取输入映射子系统
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(InputMappingContext, 0);

        UEnhancedInputComponent* EnhancedInputCom = Cast<UEnhancedInputComponent>(InputComponent);
        EnhancedInputCom->BindAction(PauseGameAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::PauseGame);

        UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
    }
}

void AFPSPlayerController::PauseGame()
{
    //SetGamePause()
    if (MenuWidgetClass)
    {
        MenuWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
        if (MenuWidgetInstance)
        {
            MenuWidgetInstance->AddToViewport();

            FInputModeGameAndUI InputModeData;
            SetInputMode(InputModeData);
            SetShowMouseCursor(true);

            SetPause(true);
        }
    }
}