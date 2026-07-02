#include "KeyEscapeFireArea.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "KeyEscapeEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/AudioComponent.h"

AKeyEscapeFireArea::AKeyEscapeFireArea()
{
    PrimaryActorTick.bCanEverTick = true;

    DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));
    RootComponent = DamageSphere;

    DamageSphere->SetSphereRadius(250.0f);
    DamageSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DamageSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
    DamageSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    DamageSphere->SetGenerateOverlapEvents(true);

    FireVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireVisual"));
    FireVisual->SetupAttachment(DamageSphere);

    FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));
    FireAudioComponent->SetupAttachment(DamageSphere);
    FireAudioComponent->bAutoActivate = false;

    FireLoopSound = nullptr;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
        TEXT("/Engine/BasicShapes/Sphere.Sphere")
    );

    if (SphereMesh.Succeeded())
    {
        FireVisual->SetStaticMesh(SphereMesh.Object);
    }

    FireVisual->SetRelativeScale3D(FVector(2.5f, 2.5f, 0.25f));
    FireVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    DamagePerSecond = 35.0f;
    LifeTime = 5.0f;
    FireRadius = 250.0f;
    CurrentLifeTime = 0.0f;
}

void AKeyEscapeFireArea::BeginPlay()
{
    Super::BeginPlay();

    DamageSphere->SetSphereRadius(FireRadius);

    if (FireLoopSound && FireAudioComponent)
    {
        FireAudioComponent->SetSound(FireLoopSound);
        FireAudioComponent->Play();
    }
}

void AKeyEscapeFireArea::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    CurrentLifeTime += DeltaSeconds;

    DamageOverlappingEnemies(DeltaSeconds);

    if (CurrentLifeTime >= LifeTime)
    {
        Destroy();
    }
}

void AKeyEscapeFireArea::DamageOverlappingEnemies(float DeltaSeconds)
{
    TArray<AActor*> OverlappingActors;

    DamageSphere->GetOverlappingActors(
        OverlappingActors,
        AKeyEscapeEnemy::StaticClass()
    );

    for (AActor* Actor : OverlappingActors)
    {
        AKeyEscapeEnemy* Enemy = Cast<AKeyEscapeEnemy>(Actor);

        if (Enemy)
        {
            UGameplayStatics::ApplyDamage(
                Enemy,
                DamagePerSecond * DeltaSeconds,
                nullptr,
                this,
                nullptr
            );
        }
    }
}