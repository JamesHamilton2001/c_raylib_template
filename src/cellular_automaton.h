#ifndef GRID_H
#define GRID_H

#include <stddef.h>
#include <stdint.h>

#include "raylib.h"



typedef struct _CellularAutomaton CellularAutomaton;

typedef uint32_t CellState;



typedef enum _CellularAutomatonType {

    CellularAutomatonTypeGameOfLife,
    CellularAutomatonTypeBriansBrain,

    CELLULAR_AUTOMATON_TYPE_COUNT

} CellularAutomatonType;



typedef struct _CellularAutomatonTypeParameters {

    uint32_t stateCount;
    const Color * initStateColours;
    void ( *initStateFunc )( CellularAutomaton * );
    void ( *updateStateFunc )( CellularAutomaton * );
    void ( *updatePixelDataFunc )( CellularAutomaton * );

} CellularAutomatonTypeParameters;



typedef struct _CellularAutomaton {

    char name [ 128 ];
    CellularAutomatonType type;
    CellularAutomatonTypeParameters typeParams;

    int32_t rows;
    int32_t cols;
    uint32_t count;

    CellState * newStates;
    CellState * oldStates;
    CellState * stateBuffer [ 2 ];

    Color * stateColours;

    Color * pixelData;
    Image image;
    Texture2D texture;

    uint64_t iterationCount;

    uint32_t id;
    int32_t seed;
    CellState * initialStates;

} CellularAutomaton;



void CellularAutomatonInit(
    CellularAutomaton * ptr,
    CellularAutomatonType type,
    int32_t rows,
    int32_t cols,
    const char * name,
    uint32_t id,
    uint32_t seed,
    CellState * initialStates
);

void CellularAutomatonDenit( CellularAutomaton * ptr );

void CellularAutomatonUpdate( CellularAutomaton * ptr );

void CellularAutomatonDraw( const CellularAutomaton * ptr );



#endif // #ifndef GRID_H