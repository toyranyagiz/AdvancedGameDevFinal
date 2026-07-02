#include "KeyEscapeEnemy.h"

#include "AIController.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KeyEscapeCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"
#include "UObject/ConstructorHelpers.h"

AKeyEscapeEnemy::AKeyEscapeEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AAIController::StaticClass();

    bUseControllerRotationYaw = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

    EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
    EnemyMesh->SetupAttachment(GetRootComponent());

    EnemyMovementAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EnemyMovementAudioComponent"));
    EnemyMovementAudioComponent->SetupAttachment(GetRootComponent());
    EnemyMovementAudioComponent->bAutoActivate = false;
    EnemyMovementAudioComponent->bStopWhenOwnerDestroyed = true;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );

    if (CubeMesh.Succeeded())
    {
        EnemyMesh->SetStaticMesh(CubeMesh.Object);
    }

    EnemyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
    EnemyMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.8f));
    EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    PatrolTag = FName("EnemyPatrol");

    PatrolAcceptanceRadius = 180.0f;

    PatrolSpeed = 180.0f;
    ChaseSpeed = 450.0f;

    SightRange = 900.0f;
    LoseSightRange = 1500.0f;

    AttackRange = 130.0f;
    AttackDamage = 25.0f;
    AttackCooldown = 1.0f;

    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    CurrentPatrolIndex = 0;
    bIsChasing = false;
    bCanAttack = true;
    bIsDead = false;
    TargetPlayer = nullptr;

    EnemyAttackMontage = nullptr;
    EnemyDeathMontage = nullptr;
    DeathDestroyDelay = 3.0f;

    EnemyMovementLoopSound = nullptr;
    EnemyAttackSound = nullptr;
    EnemyDeathSound = nullptr;

    PatrolSoundVolume = 0.12f;
    ChaseSoundVolume = 0.60f;
}

void AKeyEscapeEnemy::BeginPlay()
{
    Super::BeginPlay();

    CurrentHealth = MaxHealth;

    bIsDead = false;

    GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;

    FindPatrolPoints();

    if (GEngine)
    {
        FString Message = FString::Printf(
            TEXT("Found patrol points: %d"),
            PatrolPoints.Num()
        );

        GEngine->AddOnScreenDebugMessage(
            -1,
            5.0f,
            FColor::Yellow,
            Message
        );
    }

    if (EnemyMovementLoopSound && EnemyMovementAudioComponent)
    {
        EnemyMovementAudioComponent->SetSound(EnemyMovementLoopSound);
        EnemyMovementAudioComponent->SetVolumeMultiplier(PatrolSoundVolume);
        EnemyMovementAudioComponent->Play();
    }

    MoveToCurrentPatrolPoint();
}

void AKeyEscapeEnemy::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (bIsDead)
    {
        return;
    }

    AKeyEscapeCharacter* PlayerCharacter = Cast<AKeyEscapeCharacter>(
        UGameplayStatics::GetPlayerCharacter(this, 0)
    );

    if (!PlayerCharacter || PlayerCharacter->IsDead())
    {
        StopChase();
        return;
    }

    float DistanceToPlayer = FVector::Dist2D(
        GetActorLocation(),
        PlayerCharacter->GetActorLocation()
    );

    if (!bIsChasing && DistanceToPlayer <= SightRange)
    {
        StartChase(PlayerCharacter);
    }

    if (bIsChasing && DistanceToPlayer > LoseSightRange)
    {
        StopChase();
    }

    if (bIsChasing)
    {
        UpdateChase();
    }
    else
    {
        UpdatePatrol();
    }
}

void AKeyEscapeEnemy::FindPatrolPoints()
{
    PatrolPoints.Empty();

    UGameplayStatics::GetAllActorsWithTag(
        this,
        PatrolTag,
        PatrolPoints
    );

    PatrolPoints.Sort([](const AActor& A, const AActor& B)
        {
            return A.GetName() < B.GetName();
        });

    CurrentPatrolIndex = 0;
}

float AKeyEscapeEnemy::TakeDamage(
    float DamageAmount,
    FDamageEvent const& DamageEvent,
    AController* EventInstigator,
    AActor* DamageCauser
)
{
    if (DamageAmount <= 0.0f || bIsDead)
    {
        return 0.0f;
    }

    CurrentHealth -= DamageAmount;

    if (GEngine)
    {
        FString Message = FString::Printf(
            TEXT("Enemy Health: %.0f / %.0f"),
            CurrentHealth,
            MaxHealth
        );

        GEngine->AddOnScreenDebugMessage(
            -1,
            1.5f,
            FColor::Red,
            Message
        );
    }

    if (CurrentHealth <= 0.0f)
    {
        Die();
    }

    return DamageAmount;
}

void AKeyEscapeEnemy::UpdatePatrol()
{
    if (PatrolPoints.Num() == 0)
    {
        return;
    }

    if (!PatrolPoints.IsValidIndex(CurrentPatrolIndex))
    {
        CurrentPatrolIndex = 0;
    }

    AActor* CurrentPoint = PatrolPoints[CurrentPatrolIndex];

    if (!CurrentPoint)
    {
        return;
    }

    float DistanceToPoint = FVector::Dist2D(
        GetActorLocation(),
        CurrentPoint->GetActorLocation()
    );

    AAIController* EnemyAIController = GetEnemyAIController();

    bool bMoveIsIdle = false;

    if (EnemyAIController)
    {
        bMoveIsIdle = EnemyAIController->GetMoveStatus() == EPathFollowingStatus::Idle;
    }

    if (DistanceToPoint <= PatrolAcceptanceRadius || bMoveIsIdle)
    {
        CurrentPatrolIndex++;

        if (CurrentPatrolIndex >= PatrolPoints.Num())
        {
            CurrentPatrolIndex = 0;
        }

        if (GEngine)
        {
            FString Message = FString::Printf(
                TEXT("Going to patrol point: %d"),
                CurrentPatrolIndex + 1
            );

            GEngine->AddOnScreenDebugMessage(
                -1,
                1.2f,
                FColor::Cyan,
                Message
            );
        }
    }

    MoveToCurrentPatrolPoint();
}

void AKeyEscapeEnemy::UpdateChase()
{
    if (!TargetPlayer)
    {
        StopChase();
        return;
    }

    MoveToPlayer();
    TryAttack();
}

void AKeyEscapeEnemy::MoveToCurrentPatrolPoint()
{
    if (PatrolPoints.Num() == 0)
    {
        return;
    }

    if (!PatrolPoints.IsValidIndex(CurrentPatrolIndex))
    {
        CurrentPatrolIndex = 0;
    }

    AActor* CurrentPoint = PatrolPoints[CurrentPatrolIndex];

    if (!CurrentPoint)
    {
        return;
    }

    AAIController* EnemyAIController = GetEnemyAIController();

    if (EnemyAIController)
    {
        EnemyAIController->MoveToLocation(
            CurrentPoint->GetActorLocation(),
            PatrolAcceptanceRadius,
            true,
            true,
            true
        );
    }
}

void AKeyEscapeEnemy::MoveToPlayer()
{
    if (!TargetPlayer)
    {
        return;
    }

    AAIController* EnemyAIController = GetEnemyAIController();

    if (EnemyAIController)
    {
        EnemyAIController->MoveToLocation(
            TargetPlayer->GetActorLocation(),
            AttackRange * 0.8f,
            true,
            true,
            true
        );
    }
}

void AKeyEscapeEnemy::StartChase(AKeyEscapeCharacter* PlayerCharacter)
{
    if (!PlayerCharacter)
    {
        return;
    }

    bIsChasing = true;
    TargetPlayer = PlayerCharacter;

    GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;

    if (EnemyMovementAudioComponent)
    {
        if (EnemyMovementLoopSound && !EnemyMovementAudioComponent->IsPlaying())
        {
            EnemyMovementAudioComponent->SetSound(EnemyMovementLoopSound);
            EnemyMovementAudioComponent->Play();
        }

        EnemyMovementAudioComponent->SetVolumeMultiplier(ChaseSoundVolume);
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.5f,
            FColor::Red,
            TEXT("Enemy started chase")
        );
    }

    MoveToPlayer();
}

void AKeyEscapeEnemy::StopChase()
{
    if (!bIsChasing)
    {
        return;
    }

    bIsChasing = false;
    TargetPlayer = nullptr;

    GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;

    if (EnemyMovementAudioComponent && EnemyMovementAudioComponent->IsPlaying())
    {
        EnemyMovementAudioComponent->SetVolumeMultiplier(PatrolSoundVolume);
    }

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.5f,
            FColor::Yellow,
            TEXT("Enemy returned to patrol")
        );
    }

    MoveToCurrentPatrolPoint();
}

void AKeyEscapeEnemy::TryAttack()
{
    if (!bCanAttack)
    {
        return;
    }

    if (!TargetPlayer)
    {
        return;
    }

    float DistanceToPlayer = FVector::Dist2D(
        GetActorLocation(),
        TargetPlayer->GetActorLocation()
    );

    if (DistanceToPlayer > AttackRange)
    {
        return;
    }

    TargetPlayer->TakePlayerDamage(AttackDamage);

    if (EnemyAttackSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            EnemyAttackSound,
            GetActorLocation()
        );
    }

    PlayAttackMontage();

    bCanAttack = false;

    GetWorldTimerManager().SetTimer(
        AttackCooldownTimerHandle,
        this,
        &AKeyEscapeEnemy::ResetAttack,
        AttackCooldown,
        false
    );

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            -1,
            1.5f,
            FColor::Red,
            TEXT("Enemy attacked")
        );
    }
}

void AKeyEscapeEnemy::ResetAttack()
{
    bCanAttack = true;
}

AAIController* AKeyEscapeEnemy::GetEnemyAIController() const
{
    return Cast<AAIController>(GetController());
}

void AKeyEscapeEnemy::PlayAttackMontage()
{
    if (!EnemyAttackMontage)
    {
        return;
    }

    USkeletalMeshComponent* SkeletalMesh = GetMesh();

    if (!SkeletalMesh)
    {
        return;
    }

    UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

    if (AnimInstance)
    {
        AnimInstance->Montage_Play(EnemyAttackMontage);
    }
}

void AKeyEscapeEnemy::Die()
{
    if (bIsDead)
    {
        return;
    }

    bIsDead = true;

    if (EnemyMovementAudioComponent)
    {
        EnemyMovementAudioComponent->Stop();
    }

    if (EnemyDeathSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,
            EnemyDeathSound,
            GetActorLocation()
        );
    }

    bIsChasing = false;
    bCanAttack = false;
    TargetPlayer = nullptr;

    GetWorldTimerManager().ClearTimer(AttackCooldownTimerHandle);

    AAIController* EnemyAIController = GetEnemyAIController();

    if (EnemyAIController)
    {
        EnemyAIController->StopMovement();
    }

    GetCharacterMovement()->DisableMovement();

    if (GetCapsuleComponent())
    {
        GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    float DestroyDelay = DeathDestroyDelay;

    if (EnemyDeathMontage)
    {
        USkeletalMeshComponent* SkeletalMesh = GetMesh();

        if (SkeletalMesh)
        {
            UAnimInstance* AnimInstance = SkeletalMesh->GetAnimInstance();

            if (AnimInstance)
            {
                float MontageLength = AnimInstance->Montage_Play(EnemyDeathMontage);

                if (MontageLength > 0.0f)
                {
                    DestroyDelay = FMath::Max(0.05f, MontageLength - 0.08f);
                }
            }
        }
    }

    GetWorldTimerManager().SetTimer(
        DeathDestroyTimerHandle,
        this,
        &AKeyEscapeEnemy::DestroyEnemy,
        DestroyDelay,
        false
    );
}

void AKeyEscapeEnemy::DestroyEnemy()
{
    Destroy();
}