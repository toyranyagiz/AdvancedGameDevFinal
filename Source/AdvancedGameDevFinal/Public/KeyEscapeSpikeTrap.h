#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "KeyEscapeSpikeTrap.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeSpikeTrap : public AActor
{
    GENERATED_BODY()

public:
    AKeyEscapeSpikeTrap();

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* DamageBox;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* TrapSound;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
    float DamageAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
    float DamageCooldown;

    bool bCanDamage;

    FTimerHandle DamageCooldownTimerHandle;

private:
    UFUNCTION()
    void OnTrapOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    void ResetDamage();
};