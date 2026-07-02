#include "KeyEscapeDoor.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "KeyEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKeyEscapeDoor::AKeyEscapeDoor()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );

    if (CubeMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMesh.Object);
    }

    Mesh->SetRelativeScale3D(FVector(0.25f, 2.0f, 2.5f));

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    RequiredKeyID = FName("RedKey");
    DoorDisplayName = FText::FromString("Door");

    OpenYaw = 90.0f;
    OpenDuration = 1.2f;

    bIsOpen = false;
    bIsOpening = false;

    CurrentOpenTime = 0.0f;
}

void AKeyEscapeDoor::BeginPlay()
{
    Super::BeginPlay();

    ClosedRotation = GetActorRotation();

    TargetOpenRotation = ClosedRotation;
    TargetOpenRotation.Yaw += OpenYaw;
}

void AKeyEscapeDoor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bIsOpening)
    {
        UpdateDoorOpening(DeltaSeconds);
    }
}

void AKeyEscapeDoor::Interact_Implementation(AActor* Interactor)
{
    if (bIsOpen || bIsOpening)
    {
        return;
    }

    AKeyEscapeCharacter* PlayerCharacter = Cast<AKeyEscapeCharacter>(Interactor);

    if (!PlayerCharacter)
    {
        return;
    }

    if (!RequiredKeyID.IsNone() && !PlayerCharacter->HasKey(RequiredKeyID))
    {
        if (GEngine)
        {
            FString Message = FString::Printf(
                TEXT("You need key: %s"),
                *RequiredKeyID.ToString()
            );

            GEngine->AddOnScreenDebugMessage(
                -1,
                1.5f,
                FColor::Red,
                Message
            );
        }

        return;
    }

    StartOpeningDoor();
}

FText AKeyEscapeDoor::GetInteractionText_Implementation() const
{
    if (bIsOpen)
    {
        return FText::FromString("Door is Open");
    }

    if (RequiredKeyID.IsNone())
    {
        return FText::FromString("Press E to Open Door");
    }

    return FText::FromString(
        FString::Printf(
            TEXT("Press E to Open %s"),
            *DoorDisplayName.ToString()
        )
    );
}

void AKeyEscapeDoor::StartOpeningDoor()
{
    bIsOpening = true;
    CurrentOpenTime = 0.0f;

    ClosedRotation = GetActorRotation();

    TargetOpenRotation = ClosedRotation;
    TargetOpenRotation.Yaw += OpenYaw;

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.5f,
            FColor::Green,
            TEXT("Door opening")
        );
    }
}

void AKeyEscapeDoor::UpdateDoorOpening(float DeltaSeconds)
{
    CurrentOpenTime += DeltaSeconds;

    float Alpha = 1.0f;

    if (OpenDuration > 0.0f)
    {
        Alpha = CurrentOpenTime / OpenDuration;
    }

    Alpha = FMath::Clamp(Alpha, 0.0f, 1.0f);

    float SmoothAlpha = FMath::InterpEaseInOut(
        0.0f,
        1.0f,
        Alpha,
        2.0f
    );

    FRotator NewRotation = FMath::Lerp(
        ClosedRotation,
        TargetOpenRotation,
        SmoothAlpha
    );

    SetActorRotation(NewRotation);

    if (Alpha >= 1.0f)
    {
        bIsOpening = false;
        bIsOpen = true;

        SetActorRotation(TargetOpenRotation);

        if (Mesh)
        {
            Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}