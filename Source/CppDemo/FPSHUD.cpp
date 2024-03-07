// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSHUD.h"

void AFPSHUD::DrawHUD()
{
    Super::DrawHUD();

    if (CrossHairTexture)
    {
        //找到画图中心点
        FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

        //偏离纹理大小的一半，以便纹理中心与画布中心对齐
        FVector2D CrossHairDrawPosition(Center.X - (CrossHairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrossHairTexture->GetSurfaceHeight() * 0.5f));

        //在中心点十字准星
        FCanvasTileItem TileItem(CrossHairDrawPosition, CrossHairTexture->Resource, FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);

    }
}




