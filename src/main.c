#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "raylib.h"

#include "brians_brain.h"



int screenWidth;
int screenHeight;
int targetFps;

Grid grid;



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

    return 0;
}



static void init( void )
{
    screenWidth = 1024;
    screenHeight = 1024;
    targetFps = 240;

    InitWindow( screenWidth, screenHeight, "c_raylib_template" );
    SetTargetFPS( targetFps );

    srand( 42 );

    briansBrainInit( &grid, 128, 128, BLACK, RED, WHITE );
}



static void denit( void )
{
    briansBrainDenit( &grid );

    CloseWindow( );
}



static void update( void )
{
    briansBrainUpdate( &grid );
}



static void draw( void )
{
    BeginDrawing( );

    ClearBackground( BLACK );

    int width = screenWidth / grid.cols;
    int height = screenHeight / grid.rows;

    for ( int row = 0; row < grid.rows; row++ )
    {
        int y = row * height;

        for ( int col = 0; col < grid.cols; col++ )
        {
            int x = col * width;

            CellState state = grid.newStates[ row * grid.cols + col ];

            DrawRectangle( x, y, width, height, grid.stateColours[ state ] );
        }
    }

    DrawFPS( 0, 0 );

    EndDrawing( );
}