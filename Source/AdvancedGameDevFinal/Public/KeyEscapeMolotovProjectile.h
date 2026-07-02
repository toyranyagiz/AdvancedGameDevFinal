#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyEscapeMolotovProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class AKeyEscapeFireArea;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeMolotovProjectile : public AActor
{
    GENERATED_BODY()

public:
    AKeyEscapeMolotovProjectile();

public:
    void LaunchProjectile(const FVector& Direction, float Speed);

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* BottleMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UProjectileMovementComponent* ProjectileMovement;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Molotov")
    TSubclassOf<AKeyEscapeFireArea> FireAreaClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Molotov")
    float AutoExplodeTime;

private:
    FTimerHandle AutoExplodeTimerHandle;

    bool bHasExploded;

private:
    UFUNCTION()
    void OnProjectileHit(
        UPrimitiveComponent* HitComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit
    );

    void Explode();
};