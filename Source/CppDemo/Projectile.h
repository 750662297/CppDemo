// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class CPPDEMO_API AProjectile : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AProjectile();

    // Called every frame
    virtual void Tick(float DeltaTime) override;

public:
    //初始化射击方向上发射物速度的函数
    void FireInDirection(const FVector& Direction);


protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    void Init();

    //当发射物击中无题时会调用的函数
    UFUNCTION()
        void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalPulse, const FHitResult& Hit);

public:
    //发射物网格体
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
        UStaticMeshComponent* ProjectileMesh;

    //发射物材质
    UPROPERTY(VisibleDefaultsOnly, Category = Movement)
        UMaterialInstanceDynamic* ProjectileMaterialInstance;

private:

    //球体碰撞组件
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
        USphereComponent* Collision;
    //发射物移动组件
    UPROPERTY(VisibleAnywhere, Category = Movement)
        UProjectileMovementComponent* ProjectileMovement;
};
