#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyEscapeInteractable.h"
#include "KeyEscapeFlashlight.generated.h"

class UStaticMeshComponent;
class USpotLightComponent;
class USceneComponent;
class AKeyEscapeCharacter;
class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeFlashlight : public AActor, public IKeyEscapeInteractable
{
    GENERATED_BODY()

public:
    AKeyEscapeFlashlight();

public:
    virtual void Tick(float DeltaSeconds) override;

    virtual void Interact_Implementation(AActor* Interactor) override;

    virtual FText GetInteractionText_Implementation() const override;

public:
    void OnPickedUp(AKeyEscapeCharacter* NewOwnerCharacter, USceneComponent* AttachPoint);

    void ToggleFlashlight();

    void AddBattery(float Amount);

    float GetCurrentBattery() const;

    float GetMaxBattery() const;

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USpotLightComponent* SpotLight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* PickupSound;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
    FText ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
    float MaxBattery;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
    float CurrentBattery;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
    float BatteryDrainRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
    bool bIsOn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
    FVector HandLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
    FRotator HandRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flashlight")
    FVector HandScale;
};