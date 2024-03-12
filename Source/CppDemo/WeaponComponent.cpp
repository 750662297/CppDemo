// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

UWeaponComponent::UWeaponComponent()
{

    //设置MuzzleOffset,在略靠近摄像机前生成发射物
    //MuzzleOffset.Set(100.0f, 0.0f, 110.0f);
    MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);


}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EEndPlayReason)
{
    if (Character == nullptr)
    {
        return;
    }

    if (APlayerController* PlayController = Cast<APlayerController>(Character->GetController()))
    {

        if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayController->GetLocalPlayer()))
        {
            EnhancedInputSubsystem->RemoveMappingContext(FireMappingContext);
        }
    }
}

void UWeaponComponent::AttackWeapon(AIMyCharacter* TargetCharacter)
{
    if (TargetCharacter == nullptr || TargetCharacter->GetHasRifle())
    {
        return;
    }

    Character = TargetCharacter;

    Character->SetWeapon(this);
    //设置附加规则
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
    Character->AfterAttackWeapon(this, AttachmentRules);

    Character->SetHasRifle(true);

    //绑定键位
    if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(FireMappingContext, 1);
        }

        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
        {
            EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UWeaponComponent::StartFire);
            EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UWeaponComponent::StopFire);
            EnhancedInputComponent->BindAction(SwitchDartleAction, ETriggerEvent::Triggered, this, &UWeaponComponent::SwitchDartle);
            EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UWeaponComponent::Reload);

            EnhancedInputComponent->BindAction(HolsteredAction, ETriggerEvent::Triggered, this, &UWeaponComponent::HolsteredWeapon);
        }
    }
}

void UWeaponComponent::Fire()
{
    if (Character == nullptr || Character->GetController() == nullptr)
    {
        return;
    }
    if (!HasBullet())
    {
        return;
    }

    //试图发射发射物
    if (ProjectileClass)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());

            const FRotator Rotation = Character->GetActorRotation();
            const FVector Location = GetOwner()->GetActorLocation() + Rotation.RotateVector(MuzzleOffset);

            FActorSpawnParameters ActorSpawnParams;
            ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

            World->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation, ActorSpawnParams);
        }
    }

    BulletNumber--;
    if (FireSound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation(), Character->GetActorRotation());
    }

    OnWeaponAfterFire.ExecuteIfBound();
}

void UWeaponComponent::StartFire()
{
    if (bIsFiring)
    {
        return;
    }

    if (!HasBullet())
    {
        return;
    }

    bIsFiring = true;
    Fire();

    if (bAllowDartle)
    {
        GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UWeaponComponent::Fire, 60.0f / 900.0f, true);
    }
}

void UWeaponComponent::StopFire()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0F, FColor::Red, "StopFire");
    bIsFiring = false;
    if (bAllowDartle && FireTimerHandle.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
    }
}

void UWeaponComponent::Aim()
{

}

void UWeaponComponent::SwitchDartle()
{
    bAllowDartle = !bAllowDartle;
}

//上弹
void UWeaponComponent::Reload()
{
    BulletNumber = MaxBulletNumber;
}

bool UWeaponComponent::HasBullet()
{
    if (BulletNumber > 0)
    {
        return true;
    }


    StopFire();

    //处理空仓时的音效

    return false;
}

void UWeaponComponent::HolsteredWeapon()
{
    if (Character == nullptr || Character->GetController() == nullptr)
    {
        return;
    }

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
    OnWeaponHolstered.ExecuteIfBound(!bIsHolstered, AttachmentRules);
    bIsHolstered = !bIsHolstered;
}