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

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
        UInputMappingContext* InputMappingContext;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
        UInputAction* PauseGameAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
        TSubclassOf<UUserWidget> MenuWidgetClass;

    UUserWidget* MenuWidgetInstance;

protected:
    void PauseGame();

protected:
    virtual void BeginPlay() override;

};
