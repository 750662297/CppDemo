// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "Components/TextBlock.h"


#include "FPSHUD.generated.h"

class UUserWidget;

/**
 *
 */
UCLASS()
class CPPDEMO_API AFPSHUD : public AHUD
{
    GENERATED_BODY()

public:
    AFPSHUD();

    //HUD绘制的主要调用
    virtual void DrawHUD() override;
    virtual void BeginPlay() override;

protected:
    //将被绘制在屏幕中心
    UPROPERTY(EditDefaultsOnly)
        UTexture2D* CrossHairTexture;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
        TSubclassOf<UUserWidget> AmmoCoundWidgetClass; //控件蓝图的类引用

    UUserWidget* AmmoCountWidgetInstance; //控件蓝图实例

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
        UTextBlock* HealthText;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD")
        UTextBlock* BulletText;


};
