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



#define ZOOM_INC_DEFAULT  0.125f
#define ZOOM_MAX_DEFAULT 32.000f



static CellAutom cellAutom;

static RenderTexture2D targetRenderTexure;
static float fade;

static Rectangle view;
static float zoom;
static float zoomInc;
static float zoomMax;

static Vector2 oldMouse;
static Vector2 newMouse;
static Vector2 deltaMouse;



static void init( void );

static void denit( void );

static void update( void );

static void draw( void );

static void resetView( void );



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
    int screenWidth = 2560, screenHeight = 1440;
    // int screenWidth = 1920, screenHeight = 1080;
    // int screenWidth = 1600, screenHeight = 900;

    int targetFps = 440;

    fade = 1.0f / 16.0f;

    newMouse = (Vector2){ 0.0f, 0.0f };
    oldMouse = (Vector2){ 0.0f, 0.0f };


    InitWindow( screenWidth, screenHeight, "c_raylib_template" );
    SetTargetFPS( targetFps );


    const char * name = "cellular automaton development";
    int32_t rows = (uint32_t)( (float)screenHeight * 0.25f );
    int32_t cols = (uint32_t)( (float)screenWidth  * 0.25f );
    uint32_t id = 0;
    uint32_t seed = 0x42424242;

    CellAutomInit( &cellAutom, CellularAutomatonType_gameOfLife,  NULL, rows, cols, name, id, seed, NULL );

    // CellAutomInit( &cellAutom, CellularAutomatonType_briansBrain, NULL, rows, cols, name, id, seed, NULL );

    // uint32_t antCount = 2;
    // uint32_t antInitPositions [ 2 ];
    // int32_t antRows [ 2 ];
    // int32_t antCols [ 2 ];
    // antRows[ 0 ] = 1 * ( rows / ( 2 * antCount ) );
    // antRows[ 1 ] = 3 * ( rows / ( 2 * antCount ) );
    // antCols[ 0 ] = 1 * ( cols / ( 2 * antCount ) );
    // antCols[ 1 ] = 3 * ( cols / ( 2 * antCount ) );
    // antInitPositions[ 0 ] = antRows[ 0 ] * cols + antCols[ 0 ];0
    // antInitPositions[ 1 ] = antRows[ 1 ] * cols + antCols[ 1 ];
    // LangtonsAntParams lap  = { .antCount=antCount, .antInitPositions=antInitPositions, .antRows=antRows, .antCols=antCols };
    // CellAutomInit( &cellAutom, CellularAutomatonType_langtonsAnt, &lap, rows, cols, name, id, seed, NULL );

    // LangtonsAntParams lap  = { .antCount=1024, .antInitPositions=NULL, .antRows=NULL, .antCols=NULL };
    // CellAutomInit( &cellAutom, CellularAutomatonType_langtonsAnt, &lap, rows, cols, name, id, seed, NULL );

    // CellAutomInit( &cellAutom, CELLULAR_AUTOMATON_TYPE_COUNT, NULL, rows, cols, name, id, seed, NULL );

    targetRenderTexure = LoadRenderTexture( cellAutom.texture.width, cellAutom.texture.height );

    resetView( );
}



static void denit( void )
{
    UnloadRenderTexture( targetRenderTexure );
    CellAutomDeinit( &cellAutom );
    CloseWindow( );
}



static void update( void )
{
    CellAutomUpdate( &cellAutom );

    oldMouse = newMouse;
    newMouse = GetMousePosition( );

    deltaMouse.x = newMouse.x - oldMouse.x;
    deltaMouse.y = newMouse.y - oldMouse.y;

    float wheel = GetMouseWheelMove( );

    if ( wheel != 0.0f )
    {
        Vector2 mouse = GetMousePosition();

        float oldZoom = zoom;
        float zoomFactor = 1.0f + wheel * zoomInc;

        zoom *= ( zoomFactor < zoomInc )
            ? zoomInc
            : zoomFactor;

        if ( zoom < zoomInc ) zoom = zoomInc;
        if ( zoom > zoomMax ) zoom = zoomMax;

        float scaleRatio = zoom / oldZoom;

        view.x = mouse.x - (mouse.x - view.x) * scaleRatio;
        view.y = mouse.y - (mouse.y - view.y) * scaleRatio;
    }

    if ( IsMouseButtonDown( MOUSE_BUTTON_LEFT ) )
    {
        view.x += deltaMouse.x;
        view.y += deltaMouse.y;
    }
}



static void draw( void )
{
    CellUpdateTexture( &cellAutom );

    Texture2D tex = targetRenderTexure.texture;

    const int   sw = (float)GetScreenWidth( );
    const int   sh = (float)GetScreenHeight( );
    view.width = tex.width * zoom;
    view.height = tex.height * zoom;

    Rectangle src = { 0.0f, 0.0f, tex.width, -tex.height, };
    Vector2   org = { 0.0f, 0.0f };
    float     rot = 0.0f;

    BeginTextureMode( targetRenderTexure );
    DrawRectangle( 0, 0, sw, sh, Fade( BLACK, fade ) );
    DrawTexture( cellAutom.texture, 0, 0, WHITE );
    EndTextureMode( );

    BeginDrawing( );
    ClearBackground( BLACK );
    DrawTexturePro( tex, src, view, org, rot, WHITE );
    DrawFPS( 0, 0 );
    EndDrawing( );
}



static void resetView( void )
{
    float sw = (float)GetScreenWidth( );
    float sh = (float)GetScreenHeight( );
    float tw = (float)targetRenderTexure.texture.width;
    float th = (float)targetRenderTexure.texture.height;

    float zoomX = sw / tw;
    float zoomY = sh / th;

    zoom = (zoomX < zoomY) ? zoomX : zoomY;

    float dw  = tw  * zoom;
    float dh = th * zoom;

    view = (Rectangle) { (sw - dw) * 0.5f, (sh - dh) * 0.5f, dw, dh };

    zoomInc = ZOOM_INC_DEFAULT;
    zoomMax = ZOOM_MAX_DEFAULT;
}
