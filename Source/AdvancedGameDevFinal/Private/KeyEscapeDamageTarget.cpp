#include "KeyEscapeDamageTarget.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

AKeyEscapeDamageTarget::AKeyEscapeDamageTarget()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );

    if (CubeMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMesh.Object);
    }

    Mesh->SetWorldScale3D(FVector(0.6f, 0.6f, 0.6f));

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
}

float AKeyEscapeDamageTarget::TakeDamage(
    float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser
)
{
    if (DamageAmount <= 0.0f)
    {
        return 0.0f;
    }

    CurrentHealth -= DamageAmount;

    if (GEngine)
    {
        FString Message = FString::Printf(
            TEXT("Target Health: %.0f / %.0f"),
            CurrentHealth,
            MaxHealth
        );

        GEngine->AddOnScreenDebugMessage(
            -1,
            1.5f,
            FColor::Red,
            Message
        );
    }

    if (CurrentHealth <= 0.0f)
    {
        Destroy();
    }

    return DamageAmount;
}