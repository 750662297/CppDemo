// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstViewCharacter.h"


AFirstViewCharacter::AFirstViewCharacter()
{
    FirstViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstViewCamera"));
    FirstViewCamera->SetupAttachment(GetCapsuleComponent());

    //将摄像机放置在略高于眼睛上方的位置
    FirstViewCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
    //启用Pawn控制摄像机旋转
    FirstViewCamera->bUsePawnControlRotation = true;

    FirstViewMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstViewMesh"));
    FirstViewMesh->SetOnlyOwnerSee(true);
    FirstViewMesh->SetupAttachment(FirstViewCamera);

    //禁用某些环境阴影以便实现只有单个网格体的感觉
    FirstViewMesh->bCastDynamicShadow = false;
    FirstViewMesh->CastShadow = false;
}

// 已调用每个帧
void AFirstViewCharacter::Tick(float DeltaTime)
{
    AIMyCharacter::Tick(DeltaTime);
}

// 调用以绑定要输出的功能
void AFirstViewCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    AIMyCharacter::SetupPlayerInputComponent(PlayerInputComponent);
}


void AFirstViewCharacter::BeginPlay()
{
    AIMyCharacter::BeginPlay();

    GetMesh()->SetOwnerNoSee(true);
}

void AFirstViewCharacter::AfterAttackWeapon(UWeaponComponent* TargetWeapon, const FAttachmentTransformRules& Rules, bool other)
{
    TargetWeapon->AttachToComponent(GetFirstViewMesh(), Rules, FName(TEXT("GripPoint")));
    TargetWeapon->OnWeaponAfterFire.BindUObject(this, &AFirstViewCharacter::AfterWeaponFire);
}

void AFirstViewCharacter::AfterWeaponFire()
{
    if (FireAnimation != nullptr)
    {
        UAnimInstance* AnimInstance = GetFirstViewMesh()->GetAnimInstance();
        if (AnimInstance != nullptr)
        {
            AnimInstance->Montage_Play(FireAnimation, 1.f);
        }
    }

}