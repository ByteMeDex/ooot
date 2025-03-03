#pragma once
#define Z_BG_SPOT07_TAKI_H

#include "ultra64.h"
#include "global.h"

struct BgSpot07Taki;

typedef void (*BgSpot07TakiActionFunc)(struct BgSpot07Taki*, GlobalContext*);


struct BgSpot07Taki {

    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x0164 */ BgSpot07TakiActionFunc  actionFunc;
}; 


