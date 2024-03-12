// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "IMyCharacter.generated.h"

class UWeaponComponent;

UCLASS(Config = Game)
class CPPDEMO_API AIMyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    AIMyCharacter();

public:
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

    //疾跑
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        UInputAction* RunAction;

    //转视角
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        UInputAction* LookAction;

    //跳跃
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
        UInputAction* JumpAction;

    //下蹲
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPreserveRatio = "true"))
        UInputAction* CrouchAction;

    //趴下
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPreserveRatio = "true"))
        UInputAction* ProneAction;

    //武器相关设置
    UFUNCTION(BlueprintCallable, Category = Weapon)
        bool GetHasRifle() { return IsHasRifle; }

    UFUNCTION(BlueprintCallable, Category = Weapon)
        void SetHasRifle(bool Value) { IsHasRifle = Value; }

    // 趴下相关设置
    UFUNCTION(BlueprintCallable, Category = Input)
        bool GetProneState() { return IsProne; }

    UFUNCTION(BlueprintCallable, Category = Input)
        void SetProneState(bool Value) { IsProne = Value; }


    //武器
    void SetWeapon(UWeaponComponent* component) { Weapon01 = component; }
    UWeaponComponent* GetWeapon() { return Weapon01; }

    virtual void AfterAttackWeapon(UWeaponComponent* TargetWeapon, const FAttachmentTransformRules& Rules, bool other = false) {}
    virtual void HolsteredWeapon(bool IsPackUp, const FAttachmentTransformRules& Rules) {}

    UFUNCTION(BlueprintCallable, Category = Weapon)
        bool IsHolsteredWeapon()
    {
        return bIsHolsteredWeapon;
    }

    void SetIsHolsteredWeapon(bool Value) { bIsHolsteredWeapon = Value; }


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

protected:
    enum class Enum_Run_Mode :uint8
    {
        PRESS_KEEP_RUN = 1, //按住保持奔跑
        SWITCH_RUN //按下切换奔跑
    };

    enum class Enum_Crouch_Mode :uint8
    {
        PRESS_KEEP_CROUCH = 1,
        SWITCH_CROUCH
    };

    void Init();

    //增强输入的移动，转视角
    virtual void Move(const FInputActionValue& Value);
    virtual void Look(const FInputActionValue& Value);
    virtual void StartRun();
    virtual void StopRun();

    //蹲下
    virtual void StartCrouch();
    virtual void StopCrouch();

    //趴下
    virtual void StartProne();
    virtual void StopProne();

    virtual void ChangeSpeed(bool ToRunning = false);

    void SetRunMode(Enum_Run_Mode Mode) { RunMode = Mode; }

    Enum_Run_Mode GetRunMode() { return RunMode; }

    void SetCrouchMode(Enum_Crouch_Mode Mode) { CrouchMode = Mode; }
    Enum_Crouch_Mode GetCrouchMode() { return CrouchMode; }

private:
    bool IsFirstCamera = true;
    bool IsHasRifle = false;
    bool IsProne = false; //趴下
    bool IsRunning = false;
    bool IsCrouching = false;
    bool bIsHolsteredWeapon = true;

    float CurrentSpeed = 0.0f;
    float MaxSpeed = 662.0f;


    Enum_Run_Mode RunMode = Enum_Run_Mode::SWITCH_RUN;
    Enum_Crouch_Mode CrouchMode = Enum_Crouch_Mode::PRESS_KEEP_CROUCH;


    UWeaponComponent* Weapon01 = nullptr;


};
