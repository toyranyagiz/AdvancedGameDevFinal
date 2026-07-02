#include "KeyEscapeMolotovProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "KeyEscapeFireArea.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"

AKeyEscapeMolotovProjectile::AKeyEscapeMolotovProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;

    CollisionSphere->SetSphereRadius(15.0f);
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionSphere->SetCollisionResponseToAllChannels(ECR_Block);
    CollisionSphere->SetNotifyRigidBodyCollision(true);

    BottleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BottleMesh"));
    BottleMesh->SetupAttachment(CollisionSphere);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(
        TEXT("/Engine/BasicShapes/Cylinder.Cylinder")
    );

    if (CylinderMesh.Succeeded())
    {
        BottleMesh->SetStaticMesh(CylinderMesh.Object);
    }

    BottleMesh->SetRelativeScale3D(FVector(0.12f, 0.12f, 0.35f));
    BottleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 1200.0f;
    ProjectileMovement->MaxSpeed = 1200.0f;
    ProjectileMovement->ProjectileGravityScale = 1.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;

    AutoExplodeTime = 4.0f;
    bHasExploded = false;
}

void AKeyEscapeMolotovProjectile::BeginPlay()
{
    Super::BeginPlay();

    CollisionSphere->OnComponentHit.AddDynamic(
        this,
        &AKeyEscapeMolotovProjectile::OnProjectileHit
    );

    GetWorldTimerManager().SetTimer(
        AutoExplodeTimerHandle,
        this,
        &AKeyEscapeMolotovProjectile::Explode,
        AutoExplodeTime,
        false
    );
}

void AKeyEscapeMolotovProjectile::LaunchProjectile(const FVector& Direction, float Speed)
{
    if (ProjectileMovement)
    {
        ProjectileMovement->Velocity = Direction * Speed;
    }
}

void AKeyEscapeMolotovProjectile::OnProjectileHit(
    UPrimitiveComponent* HitComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    FVector NormalImpulse,
    const FHitResult& Hit
)
{
    Explode();
}

void AKeyEscapeMolotovProjectile::Explode()
{
    if (bHasExploded)
    {
        return;
    }

    bHasExploded = true;

    if (FireAreaClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = GetOwner();

        GetWorld()->SpawnActor<AKeyEscapeFireArea>(
            FireAreaClass,
            GetActorLocation(),
            FRotator::ZeroRotator,
            SpawnParams
        );
    }

    Destroy();
}