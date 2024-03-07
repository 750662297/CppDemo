// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    Init();

    //3秒后删除发射物
    InitialLifeSpan = 3.0f;
}

void AProjectile::Init()
{
    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileScene"));
    }
    if (!Collision)
    {
        //用球体进行简单的碰撞展示
        Collision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        //将球体的碰撞配置文件名称设置为"Projectile"
        Collision->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
        // 组件击中某物时调用的事件。
        Collision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
        Collision->InitSphereRadius(15.0f);
        RootComponent = Collision;
    }

    if (!ProjectileMovement)
    {
        //使用此组件驱动发射物的移动
        ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
        ProjectileMovement->SetUpdatedComponent(Collision);
        ProjectileMovement->InitialSpeed = 9000.0f;
        ProjectileMovement->MaxSpeed = 9000.0f;
        ProjectileMovement->bRotationFollowsVelocity = true;
        ProjectileMovement->bShouldBounce = true;
        ProjectileMovement->Bounciness = 0.3f;
        ProjectileMovement->ProjectileGravityScale = 0.0f;
    }

    //设置静态网格体
    if (!ProjectileMesh)
    {
        ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Mesh/Sphere.Sphere'"));
        if (Mesh.Succeeded())
        {
            ProjectileMesh->SetStaticMesh(Mesh.Object);
        }
    }

    //设置静态网格体材质
    static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Material/ShereMaterial.ShereMaterial'"));
    if (Material.Succeeded())
    {
        ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMesh);
    }

    ProjectileMesh->SetMaterial(0, ProjectileMaterialInstance);
    ProjectileMesh->SetRelativeScale3D(FVector3d(0.09f, 0.09f, 0.09f));
    ProjectileMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AProjectile::FireInDirection(const FVector& Direction)
{
    ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalPulse, const FHitResult& Hit)
{
    if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
    {
        OtherComponent->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
    }

    Destroy();
}