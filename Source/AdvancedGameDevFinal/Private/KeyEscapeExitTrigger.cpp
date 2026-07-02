#include "KeyEscapeExitTrigger.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "KeyEscapeCharacter.h"
#include "Kismet/GameplayStatics.h"

AKeyEscapeExitTrigger::AKeyEscapeExitTrigger()
{
    PrimaryActorTick.bCanEverTick = false;

    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = TriggerBox;

    TriggerBox->SetBoxExtent(FVector(120.0f, 120.0f, 120.0f));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    TriggerBox->SetGenerateOverlapEvents(true);

    RequiredKeyID = NAME_None;
    MainMenuLevelName = FName("L_MainMenu");
    ReturnToMainMenuDelay = 3.0f;
    bHasTriggered = false;
}

void AKeyEscapeExitTrigger::BeginPlay()
{
    Super::BeginPlay();

    TriggerBox->OnComponentBeginOverlap.AddDynamic(
        this,
        &AKeyEscapeExitTrigger::OnExitOverlap
    );
}

void AKeyEscapeExitTrigger::OnExitOverlap(
    UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (bHasTriggered)
    {
        return;
    }

    AKeyEscapeCharacter* PlayerCharacter = Cast<AKeyEscapeCharacter>(OtherActor);

    if (!PlayerCharacter)
    {
        return;
    }

    if (!RequiredKeyID.IsNone() && !PlayerCharacter->HasKey(RequiredKeyID))
    {
        return;
    }

    bHasTriggered = true;

    PlayerCharacter->GetCharacterMovement()->StopMovementImmediately();
    PlayerCharacter->GetCharacterMovement()->DisableMovement();

    APlayerController* PlayerController = Cast<APlayerController>(
        PlayerCharacter->GetController()
    );

    if (PlayerController)
    {
        ShowWinScreen(PlayerController);
    }

    GetWorldTimerManager().SetTimer(
        ReturnToMainMenuTimerHandle,
        this,
        &AKeyEscapeExitTrigger::OpenMainMenu,
        ReturnToMainMenuDelay,
        false
    );
}

void AKeyEscapeExitTrigger::ShowWinScreen(APlayerController* PlayerController)
{
    if (!PlayerController)
    {
        return;
    }

    if (WinScreenWidgetClass)
    {
        UUserWidget* WinWidget = CreateWidget<UUserWidget>(
            PlayerController,
            WinScreenWidgetClass
        );

        if (WinWidget)
        {
            WinWidget->AddToViewport();
        }
    }

    PlayerController->SetInputMode(FInputModeUIOnly());
    PlayerController->bShowMouseCursor = true;
}

void AKeyEscapeExitTrigger::OpenMainMenu()
{
    if (!MainMenuLevelName.IsNone())
    {
        UGameplayStatics::OpenLevel(this, MainMenuLevelName);
    }
}