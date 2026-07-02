#include "KeyEscapeFlashlight.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SpotLightComponent.h"
#include "KeyEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


AKeyEscapeFlashlight::AKeyEscapeFlashlight()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
    SpotLight->SetupAttachment(Mesh);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMesh(
        TEXT("/Engine/BasicShapes/Cylinder.Cylinder")
    );

    if (CylinderMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CylinderMesh.Object);
    }

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    SpotLight->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
    SpotLight->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
    SpotLight->Intensity = 8000.0f;
    SpotLight->AttenuationRadius = 1200.0f;
    SpotLight->InnerConeAngle = 18.0f;
    SpotLight->OuterConeAngle = 35.0f;
    SpotLight->SetVisibility(false);

    ItemName = FText::FromString("Flashlight");

    MaxBattery = 100.0f;
    CurrentBattery = 100.0f;
    BatteryDrainRate = 5.0f;
    bIsOn = false;

    PickupSound = nullptr;

    HandLocation = FVector(35.0f, -18.0f, -15.0f);
    HandRotation = FRotator(0.0f, 90.0f, 0.0f);
    HandScale = FVector(0.12f, 0.12f, 0.35f);
}

void AKeyEscapeFlashlight::BeginPlay()
{
    Super::BeginPlay();

    SpotLight->SetVisibility(false);
}

void AKeyEscapeFlashlight::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bIsOn)
    {
        CurrentBattery -= BatteryDrainRate * DeltaSeconds;

        if (CurrentBattery <= 0.0f)
        {
            CurrentBattery = 0.0f;
            bIsOn = false;
            SpotLight->SetVisibility(false);
        }
    }
}

void AKeyEscapeFlashlight::Interact_Implementation(AActor* Interactor)
{
    AKeyEscapeCharacter* Character = Cast<AKeyEscapeCharacter>(Interactor);

    if (Character)
    {
        Character->PickupFlashlight(this);

        if (PickupSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                PickupSound,
                GetActorLocation()
            );
        }
    }
}

FText AKeyEscapeFlashlight::GetInteractionText_Implementation() const
{
    return FText::Format(
        FText::FromString("Press E to Pick Up {0}"),
        ItemName
    );
}

void AKeyEscapeFlashlight::OnPickedUp(AKeyEscapeCharacter* NewOwnerCharacter, USceneComponent* AttachPoint)
{
    if (!AttachPoint)
    {
        return;
    }

    SetOwner(NewOwnerCharacter);

    SetActorEnableCollision(false);

    if (Mesh)
    {
        Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    AttachToComponent(
        AttachPoint,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale
    );

    SetActorRelativeLocation(HandLocation);
    SetActorRelativeRotation(HandRotation);
    SetActorRelativeScale3D(HandScale);
}

void AKeyEscapeFlashlight::ToggleFlashlight()
{
    if (CurrentBattery <= 0.0f)
    {
        bIsOn = false;
        SpotLight->SetVisibility(false);
        return;
    }

    bIsOn = !bIsOn;
    SpotLight->SetVisibility(bIsOn);
}

void AKeyEscapeFlashlight::AddBattery(float Amount)
{
    CurrentBattery += Amount;

    if (CurrentBattery > MaxBattery)
    {
        CurrentBattery = MaxBattery;
    }
}

float AKeyEscapeFlashlight::GetCurrentBattery() const
{
    return CurrentBattery;
}

float AKeyEscapeFlashlight::GetMaxBattery() const
{
    return MaxBattery;
}