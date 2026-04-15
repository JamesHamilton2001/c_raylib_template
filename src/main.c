#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "raylib.h"

#include "cellular_automaton.h"
#include "brians_brain.h"
#include "game_of_life.h"
#include "langtons_ant.h"




int screenWidth;
int screenHeight;
int targetFps;

float fade;

CellAuto cellularAutomaton;

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
    screenWidth = 1920, screenHeight = 1080;
    // screenWidth = 1600, screenHeight = 900;

    targetFps = 240;

    fade = 1.0f / 64.0f;

    InitWindow( screenWidth, screenHeight, "c_raylib_template" );
    SetTargetFPS( targetFps );

    targetRenderTexure = LoadRenderTexture( screenWidth, screenHeight );



    const char * name = "cellular automaton development";
    int32_t rows = (uint32_t)( (float)screenHeight * 0.25f );
    int32_t cols = (uint32_t)( (float)screenWidth  * 0.25f );
    uint32_t id = 0;
    uint32_t seed = 0x42424242;

    // uint32_t antCount = 2;
    // uint32_t antInitPositions [ 2 ];
    // int32_t antRows [ 2 ];
    // int32_t antCols [ 2 ];
    // antRows[ 0 ] = 1 * ( rows / ( 2 * antCount ) );
    // antRows[ 1 ] = 3 * ( rows / ( 2 * antCount ) );
    // antCols[ 0 ] = 1 * ( cols / ( 2 * antCount ) );
    // antCols[ 1 ] = 3 * ( cols / ( 2 * antCount ) );
    // antInitPositions[ 0 ] = antRows[ 0 ] * cols + antCols[ 0 ];
    // antInitPositions[ 1 ] = antRows[ 1 ] * cols + antCols[ 1 ];
    // LangtonsAntParams lap  = { .antCount=antCount, .antInitPositions=antInitPositions, .antRows=antRows, .antCols=antCols };
    LangtonsAntParams lap  = { .antCount=1024, .antInitPositions=NULL, .antRows=NULL, .antCols=NULL };


    // CellularAutomatonInit( &cellularAutomaton, CellularAutomatonType_briansBrain, NULL, rows, cols, name, id, seed, NULL );
    // CellularAutomatonInit( &cellularAutomaton, CellularAutomatonType_gameOfLife,  NULL, rows, cols, name, id, seed, NULL );
    CellularAutomatonInit( &cellularAutomaton, CellularAutomatonType_langtonsAnt, &lap, rows, cols, name, id, seed, NULL );
    // CellularAutomatonInit( &cellularAutomaton, CELLULAR_AUTOMATON_TYPE_COUNT,    NULL, rows, cols, name, id, seed, NULL );
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
    // ClearBackground( BLACK );

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