// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "InputAction.h"

#include "FPSPlayerController.generated.h"

class UInputMappingContext;

/**
 *
 */
UCLASS()
class CPPDEMO_API AFPSPlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    AFPSPlayerController();

    void ShowGameOverUI();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
        UInputMappingContext* InputMappingContext;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
        UInputAction* PauseGameAction;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
        UInputAction* EndGameAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
        TSubclassOf<UUserWidget> MenuWidgetClass;

    UUserWidget* MenuWidgetInstance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
        TSubclassOf<UUserWidget> GameOverWidgetClass;

    UUserWidget* GameOverWidgetInstance;

protected:
    void PauseGame();
    void EndGame();
    UUserWidget* MainMenuInstance();
    UUserWidget* GameOverInstance();

protected:
    virtual void BeginPlay() override;

};
