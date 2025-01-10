// presto3000 Chris Pawlowski

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PRT_GridTextureTest.generated.h"

UCLASS()
class PRESTORENDERTARGETPLUGIN_API APRT_GridTextureTest : public AActor
{
	GENERATED_BODY()

public:
	APRT_GridTextureTest();

protected:

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/**  Render Target */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTextureRenderTarget2D* RenderTarget;

	/**  PIE Render Target */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	UTextureRenderTarget2D* PIECreatedRenderTarget;

	// Grid settings
	UPROPERTY(EditAnywhere, Category = "Texture")
	int32 GridSize;

	UPROPERTY(EditAnywhere, Category = "Texture")
	FLinearColor GridLineColor;

	UPROPERTY(EditAnywhere, Category = "Texture")
	FLinearColor BackgroundColor;

	FLinearColor ChessboardColor1 = FLinearColor::White;
	FLinearColor ChessboardColor2 = FLinearColor::Black;

	// Button-triggered function to update the texture
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Texture")
	void UpdateGridTexture();
	void DrawGridToTexture(UTextureRenderTarget2D* RenderTarget, int32 GridSize);
	void DrawGridChessboardToTexture(UTextureRenderTarget2D* RenderTarget, int32 GridSize);
	void UpdateCell(UTextureRenderTarget2D* InRenderTarget, int32 Row, int32 Col, FLinearColor NewColor,
	                int32 InGridSize);

	FTimerHandle TimerHandle_UpdateCell;
	void UpdateRandomCell();
	void UpdateAllCellsWithRandomColors();

private:

	
};