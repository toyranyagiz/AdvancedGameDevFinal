#include "KeyEscapeSpikeTrap.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "KeyEscapeCharacter.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AKeyEscapeSpikeTrap::AKeyEscapeSpikeTrap()
{
    PrimaryActorTick.bCanEverTick = false;

    DamageAmount = 25.0f;
    DamageCooldown = 1.0f;
    bCanDamage = true;

    DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
    RootComponent = DamageBox;

    DamageBox->SetBoxExtent(FVector(60.0f, 60.0f, 40.0f));
    DamageBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    DamageBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    DamageBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    DamageBox->SetGenerateOverlapEvents(true);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(DamageBox);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );

    if (CubeMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMesh.Object);
    }

    Mesh->SetRelativeScale3D(FVector(1.2f, 1.2f, 0.2f));
    Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, -30.0f));

    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    TrapSound = nullptr;

}

void AKeyEscapeSpikeTrap::BeginPlay()
{
    Super::BeginPlay();

    DamageBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &AKeyEscapeSpikeTrap::OnTrapOverlap
    );
}

void AKeyEscapeSpikeTrap::OnTrapOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (!bCanDamage)
    {
        return;
    }

    AKeyEscapeCharacter* PlayerCharacter = Cast<AKeyEscapeCharacter>(OtherActor);

    if (!PlayerCharacter)
    {
        return;
    }

    PlayerCharacter->TakePlayerDamage(DamageAmount);

    if (TrapSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            TrapSound,
            GetActorLocation()
        );
    }

    bCanDamage = false;

    GetWorldTimerManager().SetTimer(
        DamageCooldownTimerHandle,
        this,
        &AKeyEscapeSpikeTrap::ResetDamage,
        DamageCooldown,
        false
    );
}

void AKeyEscapeSpikeTrap::ResetDamage()
{
    bCanDamage = true;
}