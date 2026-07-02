#include "KeyEscapeNoteActor.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "KeyEscapeNoteWidget.h"
#include "Kismet/GameplayStatics.h"
#include "KeyEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKeyEscapeNoteActor::AKeyEscapeNoteActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(
        TEXT("/Engine/BasicShapes/Plane.Plane")
    );

    if (PlaneMesh.Succeeded())
    {
        Mesh->SetStaticMesh(PlaneMesh.Object);
    }

    Mesh->SetRelativeScale3D(FVector(0.4f, 0.3f, 0.1f));

    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    Mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    NoteTitle = FText::FromString("Old Note");

    NoteBody = FText::FromString(
        "The exit key is hidden somewhere inside the maze. "
        "Do not stay near the creature for too long."
    );
}

void AKeyEscapeNoteActor::Interact_Implementation(AActor* Interactor)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

    if (!PlayerController)
    {
        return;
    }

    if (!NoteWidgetClass)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                2.0f,
                FColor::Red,
                TEXT("Note Widget Class is missing")
            );
        }

        return;
    }

    UKeyEscapeNoteWidget* NoteWidget = CreateWidget<UKeyEscapeNoteWidget>(
        PlayerController,
        NoteWidgetClass
    );

    if (NoteWidget)
    {
        NoteWidget->SetupNote(NoteTitle, NoteBody);
        NoteWidget->AddToViewport();

        AKeyEscapeCharacter* Character = Cast<AKeyEscapeCharacter>(Interactor);

        if (Character)
        {
            Character->SetReadingNote(true);
        }

        FInputModeUIOnly UIInputMode;
        UIInputMode.SetWidgetToFocus(NoteWidget->TakeWidget());

        PlayerController->SetInputMode(UIInputMode);
        PlayerController->bShowMouseCursor = true;
    }
}

FText AKeyEscapeNoteActor::GetInteractionText_Implementation() const
{
    return FText::FromString("Press E to Read Note");
}