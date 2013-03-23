#include "mappers/mappers.h"
#include "mappers/chips/mmc1.h"
#include "nes/nes.h"

static void reset(int hard)
{
	nes_setsramsize(2);
	mmc1_init(mmc1_sync);
}

MAPPER(B_SxROM,reset,0,0,mmc1_state);
