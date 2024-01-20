// All Rights belong to Studio WASD 2023

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PBScoringWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class PROJECTBLEED_API UPBScoringWidget : public UUserWidget
{
	GENERATED_BODY()	

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* OnBeatAnimation;

	UFUNCTION()
	void OnScoreUpdate(int NewScore);

	UFUNCTION()
	void OnWholeBeat();

	void NativeConstruct() override;
};
