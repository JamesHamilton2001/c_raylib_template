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

CellularAutomaton cellularAutomaton;



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

    InitWindow( screenWidth, screenHeight, "c_raylib_template" );
    SetTargetFPS( targetFps );

    Color * initialStateColours = NULL;
    int32_t rows = 1024;
    int32_t cols = 1024;
    uint32_t id = 0;
    uint32_t seed = 0x42424242;
    CellState * initialStates = NULL;

    briansBrainInit(
        &cellularAutomaton,
        initialStateColours,
        rows,
        cols,
        id,
        seed,
        initialStates
    );
}



static void denit( void )
{
    CloseWindow( );
}



static void update( void )
{
    CellularAutomatonUpdate( &cellularAutomaton );
}



static void draw( void )
{
    BeginDrawing( );

    ClearBackground( BLACK );

    CellularAutomatonDraw( &cellularAutomaton );

    DrawFPS( 0, 0 );

    EndDrawing( );
}