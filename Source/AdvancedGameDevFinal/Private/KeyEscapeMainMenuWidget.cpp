#include "KeyEscapeMainMenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UKeyEscapeMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GameLevelName = FName("L_Game");
    TutorialLevelName = FName("L_Tutorial");

    if (StartButton)
    {
        StartButton->OnClicked.AddDynamic(this, &UKeyEscapeMainMenuWidget::StartGame);
    }

    if (TutorialButton)
    {
        TutorialButton->OnClicked.AddDynamic(this, &UKeyEscapeMainMenuWidget::StartTutorial);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UKeyEscapeMainMenuWidget::QuitGame);
    }
}

void UKeyEscapeMainMenuWidget::StartGame()
{
    UGameplayStatics::OpenLevel(this, FName("L_Game"));
}

void UKeyEscapeMainMenuWidget::StartTutorial()
{
    UGameplayStatics::OpenLevel(this, FName("L_Tutorial"));
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