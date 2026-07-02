#include "KeyEscapeItemBase.h"

#include "Components/StaticMeshComponent.h"
#include "KeyEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AKeyEscapeItemBase::AKeyEscapeItemBase()
{
    PrimaryActorTick.bCanEverTick = false;

    PickupSound = nullptr;
    DropSound = nullptr;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );

    if (CubeMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMesh.Object);
    }

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    ItemName = FText::FromString("Item");

    HandLocation = FVector(40.0f, 10.0f, -15.0f);
    HandRotation = FRotator(0.0f, 0.0f, 0.0f);
    HandScale = FVector(0.25f, 0.25f, 0.25f);
}

void AKeyEscapeItemBase::Interact_Implementation(AActor* Interactor)
{
    AKeyEscapeCharacter* Character = Cast<AKeyEscapeCharacter>(Interactor);

    if (Character)
    {
        Character->AddRightHandItem(this);
    }
}

FText AKeyEscapeItemBase::GetInteractionText_Implementation() const
{
    return FText::Format(
        FText::FromString("Press E to Pick Up {0}"),
        ItemName
    );
}

void AKeyEscapeItemBase::OnPickedUp(AKeyEscapeCharacter* NewOwnerCharacter, USceneComponent* AttachPoint)
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

    SetActorHiddenInGame(false);

    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            PickupSound,
            GetActorLocation()
        );
    }
}

void AKeyEscapeItemBase::OnDropped(const FVector& DropLocation, const FRotator& DropRotation)
{
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    SetOwner(nullptr);

    SetActorLocation(DropLocation);
    SetActorRotation(DropRotation);

    SetActorHiddenInGame(false);

    SetActorEnableCollision(true);

    if (Mesh)
    {
        Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        Mesh->SetCollisionResponseToAllChannels(ECR_Block);
        Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    }

    if (DropSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            DropSound,
            GetActorLocation()
        );
    }
}

void AKeyEscapeItemBase::SetItemVisibleInHand(bool bVisible)
{
    SetActorHiddenInGame(!bVisible);

    if (Mesh)
    {
        Mesh->SetVisibility(bVisible, true);
    }
}

void AKeyEscapeItemBase::UseItem(AKeyEscapeCharacter* UserCharacter)
{
}

void AKeyEscapeItemBase::ReloadItem(AKeyEscapeCharacter* UserCharacter)
{
}

FText AKeyEscapeItemBase::GetItemName() const
{
    return ItemName;
}