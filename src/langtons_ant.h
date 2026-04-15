#ifndef LANGTONS_ANT
#define LANGTONS_ANT

#include <stddef.h>
#include <stdint.h>

#include "raylib.h"

#include "cellular_automaton.h"



#define LANGTONS_ANT_STR "Langtons Ant"


typedef enum _LangtonsAntState {

    LangtonsAntState_off,
    LangtonsAntState_on,

    LANGTONS_ANT_STATE_COUNT

} LangtonsAntState;


typedef struct _LangtonsAntParams {

    uint32_t antCount;

    uint32_t * antInitPositions;
    int8_t * antInitDirections;

    int32_t * antRows;
    int32_t * antCols;
    int8_t * antDirections;

} LangtonsAntParams;



extern const CellAutoTypeStaticMems langtonsAntTypeParameters;



void langtonsAntInitStateFunc( CellAuto * ptr, const CellAutoTypeParams * argsPtr );

void langtonsAntUpdateState( CellAuto * ptr );

void langtonsAntUpdatePixelData( CellAuto * ptr );

void LangtonsAntParamsDestroy( CellAutoTypeParams * vPtr );




#endif // #ifndef LANGTONS_ANT