// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/HUD.h"
#include "ThirdViewCharacter.h"
#include "FPSHUD.h"

AFPSPlayerController::AFPSPlayerController()
{
    static ConstructorHelpers::FClassFinder<UUserWidget>WidgetClass(TEXT("WidgetBlueprint'/Game/UI/UMG/UMG_MainMenu.UMG_MainMenu_C'"));
    if (WidgetClass.Succeeded())
    {
        MenuWidgetClass = WidgetClass.Class;
    }
    static ConstructorHelpers::FClassFinder<UUserWidget>GameOverClass(TEXT("WidgetBlueprint'/Game/UI/UMG/UMG_GameOver.UMG_GameOver_C'"));
    if (GameOverClass.Succeeded())
    {
        GameOverWidgetClass = GameOverClass.Class;
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
        EnhancedInputCom->BindAction(EndGameAction, ETriggerEvent::Triggered, this, &AFPSPlayerController::EndGame);
        if (PauseGameAction)
        {
            PauseGameAction->bTriggerWhenPaused = true;
        }

        UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
    }
}

void AFPSPlayerController::PauseGame()
{
    if (MenuWidgetClass == nullptr)
    {
        return;
    }

    if (MenuWidgetInstance)
    {
        MenuWidgetInstance->RemoveFromParent();
        MenuWidgetInstance = nullptr;

        FInputModeGameOnly InputModeData;
        SetInputMode(InputModeData);
        SetShowMouseCursor(false);
        SetPause(false);

        AFPSHUD* HUD = Cast<AFPSHUD>(GetHUD());
        if (HUD)
        {
            HUD->SetHide(false);
        }

        return;
    }

    if (MainMenuInstance())
    {
        MainMenuInstance()->AddToViewport();

        FInputModeGameAndUI InputModeData;
        SetInputMode(InputModeData);
        SetShowMouseCursor(true);

        AFPSHUD* HUD = Cast<AFPSHUD>(GetHUD());
        if (HUD)
        {
            HUD->SetHide(true);
        }
        SetPause(true);
    }

}

void AFPSPlayerController::EndGame()
{
    ShowGameOverUI();
}

void AFPSPlayerController::ShowGameOverUI()
{
    if (GameOverInstance())
    {
        GameOverInstance()->AddToViewport();

        FInputModeUIOnly InputModeData;
        SetInputMode(InputModeData);
        SetShowMouseCursor(true);

        SetPause(true);
    }
}

UUserWidget* AFPSPlayerController::MainMenuInstance()
{
    if (MenuWidgetClass == nullptr)
    {
        return nullptr;
    }

    if (MenuWidgetInstance == nullptr)
    {
        MenuWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
    }

    return MenuWidgetInstance;
}

UUserWidget* AFPSPlayerController::GameOverInstance()
{

    if (GameOverWidgetClass == nullptr)
    {
        return nullptr;
    }

    if (GameOverWidgetInstance == nullptr)
    {
        GameOverWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass);
    }

    return GameOverWidgetInstance;
}