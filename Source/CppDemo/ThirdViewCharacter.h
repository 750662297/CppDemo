#pragma once

#include "CoreMinimal.h"
#include "IMyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "WeaponComponent.h"

#include "ThirdViewCharacter.generated.h"

UCLASS()
class CPPDEMO_API AThirdViewCharacter : public AIMyCharacter
{
    GENERATED_BODY()

public:
    // 设置此 character 属性的默认值
    AThirdViewCharacter();

    // 已调用每个帧
    virtual void Tick(float DeltaTime) override;

    // 调用以绑定要输出的功能
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    virtual void AfterAttackWeapon(UWeaponComponent* TargetWeapon, const FAttachmentTransformRules& Rules, bool other = false) override;
    virtual void AfterWeaponFire();
    virtual void HolsteredWeapon(bool IsHolstered, const FAttachmentTransformRules& Rules);

    UFUNCTION(BlueprintCallable, Category = Weapon)
        void AttachWeaponToBack();

    UFUNCTION(BlueprintCallable, Category = Weapon)
        void AttachWeaponToHand();

protected:
    // 已在游戏开始或生成时调用
    virtual void BeginPlay() override;

    //武器射击动画
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
        UAnimMontage* FireAnimation;

    //收起武器动画
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
        UAnimMontage* HolsteredWeaponAnimation;
    //拿出武器动画
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GamePlay)
        UAnimMontage* UnHolsteredWeaponAnimation;

private:

    UPROPERTY(EditAnywhere)
        USpringArmComponent* ThirdViewSpringArm;

    UPROPERTY(VisibleAnywhere)
        UCameraComponent* ThirdViewCamera;
};
