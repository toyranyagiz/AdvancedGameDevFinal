#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyEscapeInteractable.h"
#include "KeyEscapeNoteActor.generated.h"

class UStaticMeshComponent;
class UKeyEscapeNoteWidget;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeNoteActor : public AActor, public IKeyEscapeInteractable
{
    GENERATED_BODY()

public:
    AKeyEscapeNoteActor();

public:
    virtual void Interact_Implementation(AActor* Interactor) override;

    virtual FText GetInteractionText_Implementation() const override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UKeyEscapeNoteWidget> NoteWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Note")
    FText NoteTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Note")
    FText NoteBody;
};