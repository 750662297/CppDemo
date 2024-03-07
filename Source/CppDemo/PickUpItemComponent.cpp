// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpItemComponent.h"


UPickUpItemComponent::UPickUpItemComponent()
{
    SphereRadius = 32.0f;
}

void UPickUpItemComponent::BeginPlay()
{
    Super::BeginPlay();

    OnComponentBeginOverlap.AddDynamic(this, &UPickUpItemComponent::OnSphereBeginOverlap);
}

void UPickUpItemComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACFPSCharacter* Character = Cast<ACFPSCharacter>(OtherActor);
    if (OtherActor == nullptr)
    {
        return;
    }

    OnPickUp.Broadcast(Character);
    OnComponentBeginOverlap.RemoveAll(this);
}
