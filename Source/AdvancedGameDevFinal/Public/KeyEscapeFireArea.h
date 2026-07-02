#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyEscapeFireArea.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UAudioComponent;
class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeFireArea : public AActor
{
    GENERATED_BODY()

public:
    AKeyEscapeFireArea();

public:
    virtual void Tick(float DeltaSeconds) override;

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* DamageSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* FireVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAudioComponent* FireAudioComponent;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
    float DamagePerSecond;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
    float LifeTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire")
    float FireRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* FireLoopSound;

private:
    float CurrentLifeTime;

    void DamageOverlappingEnemies(float DeltaSeconds);
};