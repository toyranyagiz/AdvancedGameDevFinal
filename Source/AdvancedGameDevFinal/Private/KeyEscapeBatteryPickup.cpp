#include "KeyEscapeBatteryPickup.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "KeyEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AKeyEscapeBatteryPickup::AKeyEscapeBatteryPickup()
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

    Mesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    BatteryAmount = 35.0f;

    PickupSound = nullptr;
}

void AKeyEscapeBatteryPickup::Interact_Implementation(AActor* Interactor)
{
    AKeyEscapeCharacter* Character = Cast<AKeyEscapeCharacter>(Interactor);

    if (Character)
    {
        Character->AddFlashlightBattery(BatteryAmount);

        if (PickupSound)
        {
            UGameplayStatics::PlaySoundAtLocation(
                this,
                PickupSound,
                GetActorLocation()
            );
        }
        Destroy();
    }
}

FText AKeyEscapeBatteryPickup::GetInteractionText_Implementation() const
{
    return FText::FromString("Press E to Pick Up Battery");
}