// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h" 
#include "IMyCharacter.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

#include "WeaponComponent.generated.h"

/**
 *武器类
 */
DECLARE_DELEGATE(FWeaponAfterFire)

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CPPDEMO_API UWeaponComponent : public USkeletalMeshComponent
{
    GENERATED_BODY()

public:
    UWeaponComponent();

public:
    FWeaponAfterFire OnWeaponAfterFire;

    int GetBulletNumber() { return BulletNumber; }

public:
    //发射物
    UPROPERTY(EditDefaultsOnly, Category = Projectile)
        TSubclassOf<class AProjectile> ProjectileClass;

    //武器的声音
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
        USoundBase* FireSound;

    //武器射击动画
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
        UAnimMontage* FireAnimation;

    //射击偏移量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
        FVector MuzzleOffset;

    //捡起武器
    UFUNCTION(BlueprintCallable, Category = "Weapon")
        void AttackWeapon(AIMyCharacter* TargetCharacter);

protected:
    //武器上下文
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputMappingContext* FireMappingContext;

    //  开火
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputAction* FireAction;

    //  瞄准
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputAction* AimAction;

    //  切换连发
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputAction* SwitchDartleAction;

    //  换弹
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        class UInputAction* ReloadAction;



    UFUNCTION()
        virtual void EndPlay(const EEndPlayReason::Type EEndPlayReason) override;

    //开始开火
    void StartFire();

    //开火
    void Fire();

    //停止开火
    void StopFire();

    //瞄准
    void Aim();


    //切换连发
    void SwitchDartle();

    //上弹
    void Reload();

    bool HasBullet();

private:
    AIMyCharacter* Character;

    bool bIsFiring = false;
    bool bAllowDartle = false; //是否允许连射

    FTimerHandle FireTimerHandle;

    int BulletNumber = 30;
    int MaxBulletNumber = 30;
};
