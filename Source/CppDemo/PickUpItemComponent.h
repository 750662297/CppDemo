// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "CFPSCharacter.h"

#include "PickUpItemComponent.generated.h"

/**
 * 掉落物，奖励等基类
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, ACFPSCharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPPDEMO_API UPickUpItemComponent : public USphereComponent
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "Interaction")
        FOnPickUp OnPickUp;

    UPickUpItemComponent();

protected:
    virtual void BeginPlay() override;

    //开始碰撞,接触的回调
    UFUNCTION()
        void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
