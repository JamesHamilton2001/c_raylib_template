#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "raylib.h"

#include "cellular_automaton.h"
#include "brians_brain.h"



int screenWidth;
int screenHeight;
int targetFps;

float fade;

CellularAutomaton cellularAutomaton;

RenderTexture2D targetRenderTexure;



static void init( void );

static void denit( void );

static void update( void );

static void draw( void );



int main()
{
    init( );

    while ( !WindowShouldClose( ) )
    {
        update( );
        draw( );
    }

    denit( );

    exit( EXIT_SUCCESS );
}



static void init( void )
{
    screenWidth = 1024;
    screenHeight = 1024;
    targetFps = 240;

    fade = 0.1f;

    InitWindow( screenWidth, screenHeight, "c_raylib_template" );
    SetTargetFPS( targetFps );

    // CellularAutomatonType type = CellularAutomatonTypeBriansBrain;
    CellularAutomatonType type = CellularAutomatonTypeGameOfLife;
    const char * name = "cellular automaton development";
    int32_t rows = 512;
    int32_t cols = 512;
    uint32_t id = 0;
    uint32_t seed = 0x42424242;
    CellState * initialStates = NULL;

    CellularAutomatonInit( &cellularAutomaton, type, rows, cols, name, id, seed, initialStates );

    targetRenderTexure = LoadRenderTexture( screenWidth, screenHeight );
}



static void denit( void )
{
    UnloadRenderTexture( targetRenderTexure );
    CellularAutomatonDenit( &cellularAutomaton );
    CloseWindow( );
}



static void update( void )
{
    CellularAutomatonUpdate( &cellularAutomaton );
}



static void draw( void )
{
    BeginTextureMode( targetRenderTexure );

    DrawRectangle( 0, 0, targetRenderTexure.texture.width, targetRenderTexure.texture.height, Fade( BLACK, fade ) );

    CellularAutomatonDraw( &cellularAutomaton );

    EndTextureMode( );


    BeginDrawing( );

    ClearBackground( BLACK );

    Rectangle src = {
        0.0f,
        0.0f,
        (float)targetRenderTexure.texture.width,
        -(float)targetRenderTexure.texture.height,
    };

    Rectangle dst = {
        0.0f,
        0.0f,
        (float)targetRenderTexure.texture.width,
        (float)targetRenderTexure.texture.height,
    };

    Vector2 origin = { 0.0f, 0.0f };

    float rotation = 0.0f;

    DrawTexturePro(
        targetRenderTexure.texture,
        src,
        dst,
        origin,
        rotation,
        WHITE
    );

    DrawFPS( 0, 0 );

    EndDrawing( );
}