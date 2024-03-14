#include "ThirdViewCharacter.h"
#include "GameFramework/PlayerState.h"

// 设置默认值
AThirdViewCharacter::AThirdViewCharacter()
{
    // 设置此 character，以调用每个帧的 Tick()。如果不需要，可以禁用此功能以提高性能。
    PrimaryActorTick.bCanEverTick = true;

    ThirdViewSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("ThirdViewSpringArm"));
    ThirdViewSpringArm->SetupAttachment(GetCapsuleComponent());
    ThirdViewSpringArm->TargetArmLength = 400.0f;

    //弹簧臂长度跟随视角旋转
    ThirdViewSpringArm->bUsePawnControlRotation = true;


    ThirdViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdViewCamera"));
    ThirdViewCamera->SetupAttachment(ThirdViewSpringArm, USpringArmComponent::SocketName);
    ThirdViewCamera->bUsePawnControlRotation = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;


    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

    //设置下蹲和疾跑的模式
    SetRunMode(AIMyCharacter::Enum_Run_Mode::SWITCH_RUN);
    SetCrouchMode(AIMyCharacter::Enum_Crouch_Mode::SWITCH_CROUCH);
}

// 已在游戏开始或生成时调用
void AThirdViewCharacter::BeginPlay()
{
    AIMyCharacter::BeginPlay();
}

// 已调用每个帧
void AThirdViewCharacter::Tick(float DeltaTime)
{
    AIMyCharacter::Tick(DeltaTime);
}

// 调用以绑定要输出的功能
void AThirdViewCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    AIMyCharacter::SetupPlayerInputComponent(PlayerInputComponent);
}

void AThirdViewCharacter::AfterAttackWeapon(UWeaponComponent* TargetWeapon, const FAttachmentTransformRules& Rules, bool other)
{
    SetWeapon(TargetWeapon);
    TargetWeapon->OnWeaponAfterFire.BindUObject(this, &AThirdViewCharacter::AfterWeaponFire);
    TargetWeapon->OnWeaponHolstered.BindUObject(this, &AThirdViewCharacter::HolsteredWeapon);

    AttachWeaponToBack();
    HolsteredWeapon(false, Rules);
}

void AThirdViewCharacter::AfterWeaponFire()
{
    if (FireAnimation != nullptr)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance != nullptr)
        {
            AnimInstance->Montage_Play(FireAnimation, 1.f);
        }
    }

}
void AThirdViewCharacter::HolsteredWeapon(bool IsHolstered, const FAttachmentTransformRules& Rules)
{
    if (IsHolstered)
    {
        if (HolsteredWeaponAnimation)
        {
            UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
            if (AnimInstance != nullptr)
            {
                AnimInstance->Montage_Play(HolsteredWeaponAnimation, 1.f);
            }
        }

    }
    else
    {
        if (UnHolsteredWeaponAnimation)
        {
            UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
            if (AnimInstance != nullptr)
            {
                AnimInstance->Montage_Play(UnHolsteredWeaponAnimation, 1.f);
            }
        }
    }

    SetIsHolsteredWeapon(IsHolstered);
}

void AThirdViewCharacter::AttachWeaponToBack()
{
    FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);

    GetWeapon()->AttachToComponent(GetMesh(), Rules, FName(TEXT("weapon_attach_back")));

}

void AThirdViewCharacter::AttachWeaponToHand()
{
    FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
    GetWeapon()->AttachToComponent(GetMesh(), Rules, FName(TEXT("weapon_attach_r")));
}