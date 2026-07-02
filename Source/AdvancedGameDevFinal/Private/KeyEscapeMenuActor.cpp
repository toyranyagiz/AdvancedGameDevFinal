#include "KeyEscapeMenuActor.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "KeyEscapeMainMenuWidget.h"
#include "Kismet/GameplayStatics.h"

AKeyEscapeMenuActor::AKeyEscapeMenuActor()
{
    PrimaryActorTick.bCanEverTick = false;

    MainMenuWidget = nullptr;
}

void AKeyEscapeMenuActor::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);

    if (!PlayerController)
    {
        return;
    }

    if (MainMenuWidgetClass)
    {
        MainMenuWidget = CreateWidget<UKeyEscapeMainMenuWidget>(
            PlayerController,
            MainMenuWidgetClass
        );

        if (MainMenuWidget)
        {
            MainMenuWidget->AddToViewport();
        }
    }

    PlayerController->SetInputMode(FInputModeUIOnly());
    PlayerController->bShowMouseCursor = true;
}