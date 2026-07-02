#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyEscapeInteractable.h"
#include "KeyEscapeDoor.generated.h"

class UStaticMeshComponent;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeDoor : public AActor, public IKeyEscapeInteractable
{
    GENERATED_BODY()

public:
    AKeyEscapeDoor();

public:
    virtual void Tick(float DeltaSeconds) override;

    virtual void Interact_Implementation(AActor* Interactor) override;

    virtual FText GetInteractionText_Implementation() const override;

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    FName RequiredKeyID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    FText DoorDisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    float OpenYaw;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
    float OpenDuration;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
    bool bIsOpen;

private:
    bool bIsOpening;

    float CurrentOpenTime;

    FRotator ClosedRotation;

    FRotator TargetOpenRotation;

private:
    void StartOpeningDoor();

    void UpdateDoorOpening(float DeltaSeconds);
};