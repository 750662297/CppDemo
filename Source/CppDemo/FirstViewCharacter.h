// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IMyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "WeaponComponent.h"

#include "FirstViewCharacter.generated.h"

/**
 *
 */
UCLASS()
class CPPDEMO_API AFirstViewCharacter : public AIMyCharacter
{
    GENERATED_BODY()

public:
    AFirstViewCharacter();

public:
    // 已调用每个帧
    virtual void Tick(float DeltaTime) override;

    // 调用以绑定要输出的功能
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    virtual void AfterAttackWeapon(UWeaponComponent* TargetWeapon, const FAttachmentTransformRules& Rules, bool other = false) override;

protected:
    virtual void BeginPlay() override;


protected:
    //武器射击动画
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
        UAnimMontage* FireAnimation;

    USkeletalMeshComponent* GetFirstViewMesh() { return FirstViewMesh; }

    void AfterWeaponFire();
private:
    UPROPERTY(VisibleAnywhere)
        UCameraComponent* FirstViewCamera;

    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
        USkeletalMeshComponent* FirstViewMesh;
};
