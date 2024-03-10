// Fill out your copyright notice in the Description page of Project Settings.


#include "IMyCharacter.h"

// Sets default values
AIMyCharacter::AIMyCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIMyCharacter::BeginPlay()
{
    Super::BeginPlay();

    //添加增强输入系统的映射
    auto PlayerController = Cast<APlayerController>(Controller);
    auto EnhancedInputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    if (EnhancedInputSystem && InputMappingContext)
    {
        EnhancedInputSystem->AddMappingContext(InputMappingContext, 0);
    }
}

// Called every frame
void AIMyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AIMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AIMyCharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AIMyCharacter::StopJumping);

        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AIMyCharacter::StartCrouch);
        EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AIMyCharacter::StopCrouch);

        EnhancedInputComponent->BindAction(ProneAction, ETriggerEvent::Started, this, &AIMyCharacter::StartProne);
        EnhancedInputComponent->BindAction(ProneAction, ETriggerEvent::Completed, this, &AIMyCharacter::StopProne);

        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AIMyCharacter::StartRun);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AIMyCharacter::StopRun);

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AIMyCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AIMyCharacter::Look);
    }
}

void AIMyCharacter::Init()
{
}

//增强输入的移动
void AIMyCharacter::Move(const FInputActionValue& Value)
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

        // add movement +
        AddMovementInput(ForwardDirection, MovementVector.X);
        AddMovementInput(RightDirection, MovementVector.Y);
    }
}

void AIMyCharacter::StartRun()
{
    if (RunMode == Enum_Run_Mode::PRESS_KEEP_RUN)
    {
        CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
        GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
        return;
    }

    ChangeSpeed(!IsRunning);
    IsRunning = !IsRunning;
}

void AIMyCharacter::StopRun()
{
    if (RunMode == Enum_Run_Mode::PRESS_KEEP_RUN)
    {
        ChangeSpeed(false);
        return;
    }
}


void AIMyCharacter::ChangeSpeed(bool ToRunning)
{
    if (ToRunning)
    {
        CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
        GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;

        return;
    }

    GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;
}

//转视角
void AIMyCharacter::Look(const FInputActionValue& Value)
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

void AIMyCharacter::StartCrouch()
{
    if (CrouchMode == Enum_Crouch_Mode::PRESS_KEEP_CROUCH)
    {
        AIMyCharacter::Crouch();
        return;
    }

    IsCrouch ? AIMyCharacter::UnCrouch() : AIMyCharacter::Crouch();
    IsCrouch = !IsCrouch;
}

void AIMyCharacter::StopCrouch()
{
    if (CrouchMode == Enum_Crouch_Mode::PRESS_KEEP_CROUCH)
    {
        AIMyCharacter::UnCrouch();
        return;
    }
}

void AIMyCharacter::StartProne()
{
    IsProne = true;
}

void AIMyCharacter::StopProne()
{
    IsProne = false;
}
