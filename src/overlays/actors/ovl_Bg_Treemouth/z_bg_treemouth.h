#pragma once
#define Z_BG_TREEMOUTH_H

#include "ultra64.h"
#include "global.h"

struct BgTreemouth;

typedef void (*BgTreemouthActionFunc)(struct BgTreemouth*, GlobalContext*);


struct BgTreemouth {

    /* 0x0000 */ DynaPolyActor dyna;
    /* 0x0164 */ char unk_164[0x4];
    /* 0x0168 */ f32 unk_168;
    /* 0x016C */ BgTreemouthActionFunc actionFunc;
}; 


