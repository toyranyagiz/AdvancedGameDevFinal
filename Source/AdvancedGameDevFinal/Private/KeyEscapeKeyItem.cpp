#include "KeyEscapeKeyItem.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "KeyEscapeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AKeyEscapeKeyItem::AKeyEscapeKeyItem()
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

    Mesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    KeyID = FName("RedKey");
    KeyDisplayName = FText::FromString("Red Key");

    PickupSound = nullptr;
}

void AKeyEscapeKeyItem::Interact_Implementation(AActor* Interactor)
{
    AKeyEscapeCharacter* PlayerCharacter = Cast<AKeyEscapeCharacter>(Interactor);

    if (!PlayerCharacter)
    {
        return;
    }

    PlayerCharacter->AddKey(KeyID);

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

FText AKeyEscapeKeyItem::GetInteractionText_Implementation() const
{
    return FText::FromString(
        FString::Printf(
            TEXT("Press E to Pick Up %s"),
            *KeyDisplayName.ToString()
        )
    );
}