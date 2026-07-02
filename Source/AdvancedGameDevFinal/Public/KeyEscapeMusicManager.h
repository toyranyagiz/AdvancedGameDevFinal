#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeyEscapeMusicManager.generated.h"

class USceneComponent;
class UAudioComponent;
class USoundBase;

UCLASS()
class ADVANCEDGAMEDEVFINAL_API AKeyEscapeMusicManager : public AActor
{
    GENERATED_BODY()

public:
    AKeyEscapeMusicManager();

protected:
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* SceneRoot;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UAudioComponent* MusicAudioComponent;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
    USoundBase* BackgroundMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
    float MusicVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
    float FadeInTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Music")
    bool bAutoPlayMusic;

public:
    UFUNCTION(BlueprintCallable, Category = "Music")
    void PlayMusic();

    UFUNCTION(BlueprintCallable, Category = "Music")
    void StopMusic(float FadeOutTime);
};