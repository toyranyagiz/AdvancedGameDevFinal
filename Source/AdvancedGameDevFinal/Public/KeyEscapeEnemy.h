#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "KeyEscapeEnemy.generated.h"

class UStaticMeshComponent;
class AKeyEscapeCharacter;
class AAIController;
class UAnimMontage;
class USoundBase;
class UAudioComponent;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    AKeyEscapeEnemy();

public:
    virtual void Tick(float DeltaSeconds) override;

    virtual float TakeDamage(
        float DamageAmount,
        struct FDamageEvent const& DamageEvent,
        AController* EventInstigator,
        AActor* DamageCauser
    ) override;

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* EnemyMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAudioComponent* EnemyMovementAudioComponent;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
    FName PatrolTag;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patrol")
    TArray<AActor*> PatrolPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol")
    float PatrolAcceptanceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float PatrolSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float ChaseSpeed;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
    float SightRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
    float LoseSightRange;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    float AttackDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
    float AttackCooldown;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
    float MaxHealth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* EnemyAttackMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* EnemyDeathMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float DeathDestroyDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* EnemyMovementLoopSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* EnemyAttackSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundBase* EnemyDeathSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float PatrolSoundVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    float ChaseSoundVolume;

private:
    int32 CurrentPatrolIndex;

    bool bIsChasing;

    bool bCanAttack;

    bool bIsDead;

    AKeyEscapeCharacter* TargetPlayer;

    FTimerHandle AttackCooldownTimerHandle;

    FTimerHandle DeathDestroyTimerHandle;

private:
    void FindPatrolPoints();

    void UpdatePatrol();

    void UpdateChase();

    void MoveToCurrentPatrolPoint();

    void MoveToPlayer();

    void StartChase(AKeyEscapeCharacter* PlayerCharacter);

    void StopChase();

    void TryAttack();

    void ResetAttack();

    void PlayAttackMontage();

    void Die();

    void DestroyEnemy();

    AAIController* GetEnemyAIController() const;
};