#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyEscapeInteractable.h"
#include "KeyEscapeAmmoPickup.generated.h"

class UStaticMeshComponent;
class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeAmmoPickup : public AActor, public IKeyEscapeInteractable
{
    GENERATED_BODY()

public:
    AKeyEscapeAmmoPickup();

public:
    virtual void Interact_Implementation(AActor* Interactor) override;

    virtual FText GetInteractionText_Implementation() const override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
    int32 AmmoAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* PickupSound;
};