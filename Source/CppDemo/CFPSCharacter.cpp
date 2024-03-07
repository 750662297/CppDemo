// Fill out your copyright notice in the Description page of Project Settings.


#include "CFPSCharacter.h"

// Sets default values
ACFPSCharacter::ACFPSCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Init();
}

void ACFPSCharacter::Init()
{
    ////阻止控制器旋转角色
    //bUseControllerRotationPitch = false;
    //bUseControllerRotationYaw = false;
    //bUseControllerRotationRoll = false;


    /*
    GetCharacterMovement()->JumpZVelocity = 700.f; // 配置角色跳跃速度
    GetCharacterMovement()->AirControl = 0.35f; //配置角色空中控制性
    GetCharacterMovement()->MaxWalkSpeed = 500.f; //配置角色最大行走速度
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
*/

//创建第一人称摄像机组件
    FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));

    //将摄像机组件附加到我们的胶囊体组件
    FPSCameraComponent->SetupAttachment(GetCapsuleComponent());

    //将摄像机放置在略高于眼睛上方的位置
    FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));

    //启用Pawn控制摄像机旋转
    FPSCameraComponent->bUsePawnControlRotation = true;

    //为所属玩家创建第一人称网格体组件
    FpsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));


    //仅所属玩家可以看见此网格体
    FpsMesh->SetOnlyOwnerSee(true);
    //将FPS网格体附加到FPS摄像机
    FpsMesh->SetupAttachment(FPSCameraComponent);

    //禁用某些环境阴影以便实现只有单个网格体的感觉
    FpsMesh->bCastDynamicShadow = false;
    FpsMesh->CastShadow = false;

    //创建第三人称相关
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    ThirdCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("ThirdCameraComp"));

    SpringArmComp->SetupAttachment(GetCapsuleComponent());
    ThirdCameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

    //设置弹簧臂的默认值
    SpringArmComp->TargetArmLength = 400.0f;

    //弹簧臂长度跟随视角旋转
    SpringArmComp->bUsePawnControlRotation = true;
    ThirdCameraComp->bUsePawnControlRotation = false;

    //设置初始视角
    CameraToFirst();
}

// Called when the game starts or when spawned
void ACFPSCharacter::BeginPlay()
{
    Super::BeginPlay();

    //添加增强输入系统的映射
    auto PlayerController = Cast<APlayerController>(Controller);
    auto EnhancedInputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    if (EnhancedInputSystem && InputMappingContext)
    {
        EnhancedInputSystem->AddMappingContext(InputMappingContext, 0);
    }

    //所属玩家看不到常规（第三人称）全身网格体
    GetMesh()->SetOwnerNoSee(true);
}

// Called every frame
void ACFPSCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// 被调用，将功能与输入绑定
void ACFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACFPSCharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACFPSCharacter::StopJumping);

        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACFPSCharacter::StartCrouch);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACFPSCharacter::StopCrouch);

        EnhancedInputComponent->BindAction(ProneAction, ETriggerEvent::Started, this, &ACFPSCharacter::StartProne);
        EnhancedInputComponent->BindAction(ProneAction, ETriggerEvent::Completed, this, &ACFPSCharacter::StopProne);




        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACFPSCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACFPSCharacter::Look);

        EnhancedInputComponent->BindAction(SwitchViewAction, ETriggerEvent::Triggered, this, &ACFPSCharacter::SwitchView);
    }
}

//增强输入的移动
void ACFPSCharacter::Move(const FInputActionValue& Value)
{
    /*
    //仅第一人称可用，
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller)
    {
        AddMovementInput(GetActorForwardVector(), MovementVector.X);
        AddMovementInput(GetActorRightVector(), MovementVector.Y);
    }
    */


    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector 
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement 
        AddMovementInput(ForwardDirection, MovementVector.X);
        AddMovementInput(RightDirection, MovementVector.Y);
    }
}

//转视角
void ACFPSCharacter::Look(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ACFPSCharacter::SwitchView()
{
    if (isFirstCamera)
    {
        CameraToThird();
    }
    else
    {
        CameraToFirst();
    }

    isFirstCamera = !isFirstCamera;
}

void ACFPSCharacter::CameraToThird()
{
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate


    FpsMesh->SetOwnerNoSee(true);
    GetMesh()->SetOwnerNoSee(false);

    FPSCameraComponent->SetActive(false);
    ThirdCameraComp->SetActive(true);
}

void ACFPSCharacter::CameraToFirst()
{
    bUseControllerRotationPitch = true;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = true;

    GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	

    FpsMesh->SetOnlyOwnerSee(true);
    FpsMesh->SetOwnerNoSee(false);
    GetMesh()->SetOwnerNoSee(true);

    FPSCameraComponent->SetActive(true);
    ThirdCameraComp->SetActive(false);
}

void ACFPSCharacter::StartCrouch()
{
    isCrouching = true;
}

void ACFPSCharacter::StopCrouch()
{
    isCrouching = false;
}

void ACFPSCharacter::StartProne()
{
    isProne = true;
}

void ACFPSCharacter::StopProne()
{
    isProne = false;
}