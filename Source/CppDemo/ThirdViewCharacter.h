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

protected:
    // 已在游戏开始或生成时调用
    virtual void BeginPlay() override;

public:
    // 已调用每个帧
    virtual void Tick(float DeltaTime) override;

    // 调用以绑定要输出的功能
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY(EditAnywhere)
        USpringArmComponent* ThirdViewSpringArm;

    UPROPERTY(VisibleAnywhere)
        UCameraComponent* ThirdViewCamera;
};
