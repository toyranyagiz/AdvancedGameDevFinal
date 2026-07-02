#include "KeyEscapeAmmoPickup.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "KeyEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AKeyEscapeAmmoPickup::AKeyEscapeAmmoPickup()
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

    Mesh->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    AmmoAmount = 7;

    PickupSound = nullptr;
}

void AKeyEscapeAmmoPickup::Interact_Implementation(AActor* Interactor)
{
    AKeyEscapeCharacter* Character = Cast<AKeyEscapeCharacter>(Interactor);

    if (!Character)
    {
        return;
    }

    bool bAdded = Character->AddPistolAmmo(AmmoAmount);

    if (bAdded)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                1.5f,
                FColor::Green,
                TEXT("Ammo picked up")
            );
        }

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

FText AKeyEscapeAmmoPickup::GetInteractionText_Implementation() const
{
    return FText::FromString("Press E to Pick Up Ammo");
}