// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"
#include "Blueprint/UserWidget.h"
#include "IMyCharacter.h"
#include "WeaponComponent.h"

AFPSHUD::AFPSHUD()
{
    static ConstructorHelpers::FClassFinder<UUserWidget>WidgetClass(TEXT("WidgetBlueprint'/Game/UI/HUD/HUD_AmmonCount.HUD_AmmonCount_C'"));
    if (WidgetClass.Succeeded())
    {
        AmmoCoundWidgetClass = WidgetClass.Class;
    }
}

void AFPSHUD::BeginPlay()
{
    Super::BeginPlay();

    if (AmmoCoundWidgetClass)
    {
        AmmoCountWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), AmmoCoundWidgetClass);

        if (AmmoCountWidgetInstance)
        {
            AmmoCountWidgetInstance->AddToViewport();
        }
    }

    if (AmmoCountWidgetInstance == nullptr)
    {
        return;
    }

    HealthText = Cast<UTextBlock>(AmmoCountWidgetInstance->GetWidgetFromName(TEXT("TextHealth")));
    BulletText = Cast<UTextBlock>(AmmoCountWidgetInstance->GetWidgetFromName(TEXT("TextBullet")));

}

void AFPSHUD::DrawHUD()
{
    Super::DrawHUD();

    //if (CrossHairTexture)
    //{
    //    //找到画图中心点
    //    FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

    //    //偏离纹理大小的一半，以便纹理中心与画布中心对齐
    //    FVector2D CrossHairDrawPosition(Center.X - (CrossHairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrossHairTexture->GetSurfaceHeight() * 0.5f));

    //    //在中心点十字准星
    //    FCanvasTileItem TileItem(CrossHairDrawPosition, CrossHairTexture->Resource, FLinearColor::White);
    //    TileItem.BlendMode = SE_BLEND_Translucent;
    //    Canvas->DrawItem(TileItem);

    //}

    if (HealthText)
    {
        HealthText->SetText(FText::FromString("100"));
    }


    AIMyCharacter* Character = Cast<AIMyCharacter>(GetOwningPawn());

    if (Character->GetHasRifle())
    {
        auto Weapon = Character->GetWeapon();
        if (BulletText && Weapon)
        {
            BulletText->SetText(FText::FromString(FString::FromInt(Weapon->GetBulletNumber())));
        }
    }

}




