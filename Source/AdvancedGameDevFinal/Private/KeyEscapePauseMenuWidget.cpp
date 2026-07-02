#include "KeyEscapePauseMenuWidget.h"

#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UKeyEscapePauseMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MainMenuLevelName.IsNone())
    {
        MainMenuLevelName = FName("L_MainMenu");
    }

    if (ResumeButton)
    {
        ResumeButton->OnClicked.AddDynamic(
            this,
            &UKeyEscapePauseMenuWidget::ResumeGame
        );
    }

    if (MainMenuButton)
    {
        MainMenuButton->OnClicked.AddDynamic(
            this,
            &UKeyEscapePauseMenuWidget::ReturnToMainMenu
        );
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(
            this,
            &UKeyEscapePauseMenuWidget::QuitGame
        );
    }
}

void UKeyEscapePauseMenuWidget::ResumeGame()
{
    APlayerController* PlayerController = GetOwningPlayer();

    if (PlayerController)
    {
        FInputModeGameOnly GameInputMode;
        PlayerController->SetInputMode(GameInputMode);
        PlayerController->bShowMouseCursor = false;
    }

    UGameplayStatics::SetGamePaused(this, false);

    RemoveFromParent();
}

void UKeyEscapePauseMenuWidget::ReturnToMainMenu()
{
    UGameplayStatics::SetGamePaused(this, false);

    UGameplayStatics::OpenLevel(
        this,
        MainMenuLevelName
    );
}

void UKeyEscapePauseMenuWidget::QuitGame()
{
    APlayerController* PlayerController = GetOwningPlayer();

    UKismetSystemLibrary::QuitGame(
        this,
        PlayerController,
        EQuitPreference::Quit,
        true
    );
}