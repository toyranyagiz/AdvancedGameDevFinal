#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyEscapeInteractable.h"
#include "KeyEscapeItemBase.generated.h"

class UStaticMeshComponent;
class AKeyEscapeCharacter;
class USceneComponent;
class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeItemBase : public AActor, public IKeyEscapeInteractable
{
    GENERATED_BODY()

public:
    AKeyEscapeItemBase();

public:
    virtual void Interact_Implementation(AActor* Interactor) override;

    virtual FText GetInteractionText_Implementation() const override;

public:
    void OnPickedUp(AKeyEscapeCharacter* NewOwnerCharacter, USceneComponent* AttachPoint);

    void OnDropped(const FVector& DropLocation, const FRotator& DropRotation);

    void SetItemVisibleInHand(bool bVisible);

    virtual void UseItem(AKeyEscapeCharacter* UserCharacter);

    virtual void ReloadItem(AKeyEscapeCharacter* UserCharacter);

    FText GetItemName() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FVector HandLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FRotator HandRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FVector HandScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* PickupSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* DropSound;
};
