#include "KeyEscapeMainMenuWidget.h"

#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UKeyEscapeMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameLevelName = FName("L_Game");

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UKeyEscapeMainMenuWidget::StartGame);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UKeyEscapeMainMenuWidget::QuitGame);
    }
}

void UKeyEscapeMainMenuWidget::StartGame()
{
    if (!GameLevelName.IsNone())
    {
        UGameplayStatics::OpenLevel(this, GameLevelName);
    }
}

void UKeyEscapeMainMenuWidget::QuitGame()
{
    APlayerController* PlayerController = GetOwningPlayer();

    UKismetSystemLibrary::QuitGame(
        this,
        PlayerController,
        EQuitPreference::Quit,
        true
    );
}