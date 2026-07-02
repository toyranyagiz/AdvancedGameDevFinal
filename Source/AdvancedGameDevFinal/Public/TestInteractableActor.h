#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyEscapeInteractable.h"
#include "TestInteractableActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API ATestInteractableActor : public AActor, public IKeyEscapeInteractable
{
    GENERATED_BODY()

public:
    ATestInteractableActor();

public:
    virtual void Interact_Implementation(AActor* Interactor) override;

    virtual FText GetInteractionText_Implementation() const override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    FText InteractionText;
};