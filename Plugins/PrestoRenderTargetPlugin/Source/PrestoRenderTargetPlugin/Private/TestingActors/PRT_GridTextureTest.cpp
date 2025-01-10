// presto3000 Chris Pawlowski


#include "TestingActors/PRT_GridTextureTest.h"

#include "CanvasItem.h"
#include "Engine/Canvas.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"

APRT_GridTextureTest::APRT_GridTextureTest()
{
    PrimaryActorTick.bCanEverTick = true;
    
    GridSize = 10;
    GridLineColor = FLinearColor::Black;
    BackgroundColor = FLinearColor::White;
    
    RenderTarget = CreateDefaultSubobject<UTextureRenderTarget2D>(TEXT("RenderTarget"));
    RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
    RenderTarget->InitAutoFormat(512, 512);
    RenderTarget->ClearColor = FLinearColor::Black;
    RenderTarget->bAutoGenerateMips = false;
}

void APRT_GridTextureTest::BeginPlay()
{
    Super::BeginPlay();

    // Assuming PIECreatedRenderTarget is already initialized
    if (PIECreatedRenderTarget)
    {
        // Initialize the texture by drawing the grid once
        DrawGridToTexture(PIECreatedRenderTarget, GridSize);
        
        GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateCell, this, &APRT_GridTextureTest::UpdateAllCellsWithRandomColors, 0.1f, true);
    }
    
    
}

void APRT_GridTextureTest::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APRT_GridTextureTest::UpdateGridTexture()
{
    if (RenderTarget)
    {
        DrawGridChessboardToTexture(RenderTarget, GridSize);
        UpdateCell(RenderTarget, 1, 1, FLinearColor::Green, GridSize);
    }
    if (PIECreatedRenderTarget)
    {
        DrawGridChessboardToTexture(PIECreatedRenderTarget, GridSize);
        UpdateCell(PIECreatedRenderTarget, 1, 1, FLinearColor::Green, GridSize);
    }
}

void APRT_GridTextureTest::DrawGridToTexture(UTextureRenderTarget2D* InRenderTarget, int32 InGridSize)
{
    if (!InRenderTarget) return;
    
    UCanvas* Canvas;
    FVector2D CanvasSize;
    FDrawToRenderTargetContext Context;
    
    UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, InRenderTarget, Canvas, CanvasSize, Context);

    if (Canvas)
    {
        FCanvasTileItem TileItem(FVector2D(0, 0), FVector2D(InRenderTarget->SizeX, InRenderTarget->SizeY), BackgroundColor);
        TileItem.BlendMode = SE_BLEND_Opaque;
        Canvas->DrawItem(TileItem);
        
        float CellWidth = CanvasSize.X / InGridSize;
        float CellHeight = CanvasSize.Y / InGridSize;

        // Draw grid lines
        FCanvasLineItem LineItem;
        LineItem.SetColor(GridLineColor);

        // Horizontal lines
        for (int32 i = 0; i <= InGridSize; ++i)
        {
            float Y = i * CellHeight;
            LineItem.Draw(Canvas->Canvas, FVector2D(0, Y), FVector2D(CanvasSize.X, Y));
        }

        // Vertical lines
        for (int32 i = 0; i <= InGridSize; ++i)
        {
            float X = i * CellWidth;
            LineItem.Draw(Canvas->Canvas, FVector2D(X, 0), FVector2D(X, CanvasSize.Y));
        }
    }
    
    UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
}

void APRT_GridTextureTest::DrawGridChessboardToTexture(UTextureRenderTarget2D* InRenderTarget, int32 InGridSize)
{
    if (!InRenderTarget) return;

    UCanvas* Canvas;
    FVector2D CanvasSize;
    FDrawToRenderTargetContext Context;
    
    UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, InRenderTarget, Canvas, CanvasSize, Context);

    if (Canvas)
    {
        // Calculate cell size
        float CellWidth = CanvasSize.X / InGridSize;
        float CellHeight = CanvasSize.Y / InGridSize;

        // Loop through rows and columns to draw the chessboard pattern
        for (int32 Row = 0; Row < InGridSize; ++Row)
        {
            for (int32 Col = 0; Col < InGridSize; ++Col)
            {
                // Determine the color for the current cell
                FLinearColor CellColor = ((Row + Col) % 2 == 0) ? ChessboardColor1 : ChessboardColor2;

                // Draw the cell
                FVector2D CellPosition(Col * CellWidth, Row * CellHeight);
                FVector2D CellSize(CellWidth, CellHeight);

                FCanvasTileItem TileItem(CellPosition, CellSize, CellColor);
                TileItem.BlendMode = SE_BLEND_Opaque;
                Canvas->DrawItem(TileItem);
            }
        }
    }
    
    UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
}

void APRT_GridTextureTest::UpdateCell(UTextureRenderTarget2D* InRenderTarget, int32 Row, int32 Col, FLinearColor NewColor, int32 InGridSize)
{
    if (!InRenderTarget) return;

    UCanvas* Canvas;
    FVector2D CanvasSize;
    FDrawToRenderTargetContext Context;
    
    UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, InRenderTarget, Canvas, CanvasSize, Context);

    if (Canvas)
    {
        float CellWidth = CanvasSize.X / InGridSize;
        float CellHeight = CanvasSize.Y / InGridSize;

        // Calculate the position and size of the cell to update
        FVector2D CellPosition(Col * CellWidth, Row * CellHeight);
        FVector2D CellSize(CellWidth, CellHeight);

        // Draw the updated cell with the new color
        FCanvasTileItem TileItem(CellPosition, CellSize, NewColor);
        TileItem.BlendMode = SE_BLEND_Opaque;
        Canvas->DrawItem(TileItem);
    }
    
    UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
}

void APRT_GridTextureTest::UpdateRandomCell()
{
    if (!PIECreatedRenderTarget) return;
    
    double StartTime = FPlatformTime::Seconds();
    
    // Select a random cell index
    int32 RandomRow = FMath::RandRange(0, GridSize - 1);
    int32 RandomColumn = FMath::RandRange(0, GridSize - 1);

    // Calculate the UV coordinates for this random cell
    float CellWidth = 1.0f / GridSize;
    float CellHeight = 1.0f / GridSize;

    // Random cell UV position (Normalized)
    FVector2D CellUV(
        RandomColumn * CellWidth,
        RandomRow * CellHeight
    );

    // Generate a random color
    FColor RandomColor = FColor(
        FMath::RandRange(0, 255),  // Random Red
        FMath::RandRange(0, 255),  // Random Green
        FMath::RandRange(0, 255),  // Random Blue
        255                        // Fully opaque
    );
    
    // ---- Begin drawing to the render target ---- //
    UCanvas* Canvas;
    FVector2D CanvasSize;
    FDrawToRenderTargetContext Context;
    
    UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, PIECreatedRenderTarget, Canvas, CanvasSize, Context);

    if (Canvas)
    {
        // Draw a colored rectangle (random cell)
        FCanvasTileItem TileItem(CellUV * CanvasSize, FVector2D(CellWidth * CanvasSize.X, CellHeight * CanvasSize.Y), RandomColor);
        TileItem.BlendMode = SE_BLEND_Opaque;
        Canvas->DrawItem(TileItem);
    }
    
    UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
    
    double ElapsedTime = FPlatformTime::Seconds() - StartTime;
    
    UE_LOG(LogTemp, Log, TEXT("Time taken to update random cell: %f seconds"), ElapsedTime);
}

void APRT_GridTextureTest::UpdateAllCellsWithRandomColors()
{
    if (!PIECreatedRenderTarget) return;
    
    double StartTime = FPlatformTime::Seconds();

    // ---- Begin drawing to the render target ---- //
    UCanvas* Canvas;
    FVector2D CanvasSize;
    FDrawToRenderTargetContext Context;
    
    UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, PIECreatedRenderTarget, Canvas, CanvasSize, Context);

    if (Canvas)
    {
        float CellWidth = 1.0f / GridSize;
        float CellHeight = 1.0f / GridSize;

        // Loop through all the cells and assign random colors
        for (int32 Row = 0; Row < GridSize; ++Row)
        {
            for (int32 Col = 0; Col < GridSize; ++Col)
            {
                // Calculate the UV coordinates for this cell
                FVector2D CellUV(
                    Col * CellWidth,
                    Row * CellHeight
                );
                
                FColor RandomColor = FColor(
                    FMath::RandRange(0, 255),  // Random Red
                    FMath::RandRange(0, 255),  // Random Green
                    FMath::RandRange(0, 255),  // Random Blue
                    255                        // Fully opaque
                );

                // Draw the cell with the random color
                FCanvasTileItem TileItem(CellUV * CanvasSize, FVector2D(CellWidth * CanvasSize.X, CellHeight * CanvasSize.Y), RandomColor);
                TileItem.BlendMode = SE_BLEND_Translucent;
                Canvas->DrawItem(TileItem);
            }
        }
    }
    
    UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, Context);
    
    double ElapsedTimeSeconds = FPlatformTime::Seconds() - StartTime;
    double ElapsedTimeMilliseconds = ElapsedTimeSeconds * 1000.0;
    
    UE_LOG(LogTemp, Log, TEXT("Time taken to update all cells: %f ms"), ElapsedTimeMilliseconds);
}