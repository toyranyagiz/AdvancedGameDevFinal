#include "KeyEscapeMusicManager.h"

#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Sound/SoundBase.h"

AKeyEscapeMusicManager::AKeyEscapeMusicManager()
{
    PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    MusicAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("MusicAudioComponent"));
    MusicAudioComponent->SetupAttachment(SceneRoot);
    MusicAudioComponent->bAutoActivate = false;

    BackgroundMusic = nullptr;

    MusicVolume = 0.5f;
    FadeInTime = 1.5f;
    bAutoPlayMusic = true;
}

void AKeyEscapeMusicManager::BeginPlay()
{
    Super::BeginPlay();

    if (bAutoPlayMusic)
    {
        PlayMusic();
    }
}

void AKeyEscapeMusicManager::PlayMusic()
{
    if (!BackgroundMusic || !MusicAudioComponent)
    {
        return;
    }

    MusicAudioComponent->SetSound(BackgroundMusic);
    MusicAudioComponent->FadeIn(FadeInTime, MusicVolume);
}

void AKeyEscapeMusicManager::StopMusic(float FadeOutTime)
{
    if (!MusicAudioComponent)
    {
        return;
    }

    MusicAudioComponent->FadeOut(FadeOutTime, 0.0f);
}