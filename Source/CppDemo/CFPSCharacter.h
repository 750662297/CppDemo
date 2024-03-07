// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "CFPSCharacter.generated.h"

UCLASS(config = Game)
class CPPDEMO_API ACFPSCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ACFPSCharacter();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

    //输入映射
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        UInputMappingContext* InputMappingContext;

    //移动
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        UInputAction* MoveAction;

    //转视角
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        UInputAction* LookAction;

    //跳跃
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        UInputAction* JumpAction;

    //切换第一人称，第三人称
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPreserveRatio = "true"))
        UInputAction* SwitchViewAction;

    //下蹲
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPreserveRatio = "true"))
        UInputAction* CrouchAction;

    //趴下
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPreserveRatio = "true"))
        UInputAction* ProneAction;


    /*射击操作相关*/

    //枪口相对于摄像机位置的偏移
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
        FVector MuzzleOffset;

    //武器相关设置
    UFUNCTION(BlueprintCallable, Category = Weapon)
        bool GetHasRifle() { return isHasRifle; }

    UFUNCTION(BlueprintCallable, Category = Weapon)
        void SetHasRifle(bool Value) { isHasRifle = Value; }

    //下蹲相关设置
    UFUNCTION(BlueprintCallable, Category = Input)
        bool GetIsCrouchState() { return isCrouching; }

    UFUNCTION(BlueprintCallable, Category = Input)
        void SetCrouchState(bool Value) { isCrouching = Value; }

    // 趴下相关设置
    UFUNCTION(BlueprintCallable, Category = Input)
        bool GetProneState() { return isProne; }

    UFUNCTION(BlueprintCallable, Category = Input)
        void SetProneState(bool Value) { isProne = Value; }



    USkeletalMeshComponent* GetFPSMesh() { return FpsMesh; }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void Init();

    //增强输入的移动，转视角
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

    //蹲下
    void StartCrouch();
    void StopCrouch();

    //趴下
    void StartProne();
    void StopProne();

    //切换视角
    UFUNCTION()
        void SwitchView();

    void CameraToThird();
    void CameraToFirst();

    //弹簧条
    UPROPERTY(EditAnywhere)
        USpringArmComponent* SpringArmComp;
    //第三人称摄像机
    UPROPERTY(EditAnywhere)
        UCameraComponent* ThirdCameraComp;

private:

    //FPS摄像机
    UPROPERTY(VisibleAnywhere)
        UCameraComponent* FPSCameraComponent;

    //第一人称网格体（手臂），仅对所属玩家可见
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
        USkeletalMeshComponent* FpsMesh;

    bool isFirstCamera = true;
    bool isHasRifle = false;
    bool isCrouching = false; //蹲下
    bool isProne = false; //趴下
};
