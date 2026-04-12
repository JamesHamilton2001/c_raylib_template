#ifndef BRIANS_BRAIN_H
#define BRIANS_BRAIN_H

#include <stddef.h>

#include "raylib.h"



typedef enum _CellState {

    CellStateDead,
    CellStateDying,
    CellStateLive,
    CELL_STATE_COUNT

} CellState;



typedef struct _Grid {

    int rows;
    int cols;
    size_t count;

    Color stateColours [ CELL_STATE_COUNT ];

    CellState * newStates;
    CellState * oldStates;

    CellState * stateBuffer [ 2 ];

} Grid;



void briansBrainInit( Grid * gridPtr,
                      int rows,
                      int cols,
                      Color initDeadColour,
                      Color initDyingColour,
                      Color initLiveColour );

void briansBrainDenit( Grid * gridPtr );

void briansBrainUpdate( Grid * gridPtr );



#endif //#ifndef BRIANS_BRAIN_H