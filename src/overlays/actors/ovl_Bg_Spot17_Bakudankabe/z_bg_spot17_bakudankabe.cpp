#define INTERNAL_SRC_OVERLAYS_ACTORS_OVL_BG_SPOT17_BAKUDANKABE_Z_BG_SPOT17_BAKUDANKABE_C
#include "actor_common.h"
/*
 * File: z_bg_spot17_bakudankabe.c
 * Overlay: ovl_Bg_Spot17_Bakudankabe
 * Description: Death Mountain Crater Bombable Wall
 */

#include "z_bg_spot17_bakudankabe.h"
#include "overlays/effects/ovl_Effect_Ss_Kakera/z_eff_ss_kakera.h"
#include "objects/object_spot17_obj/object_spot17_obj.h"
#include "objects/gameplay_field_keep/gameplay_field_keep.h"
#include "def/code_80043480.h"
#include "def/code_8006BA00.h"
#include "def/random.h"
#include "def/coss.h"
#include "def/sys_matrix.h"
#include "def/z_actor.h"
#include "def/z_bgcheck.h"
#include "def/z_effect_soft_sprite_old_init.h"
#include "def/z_lib.h"
#include "def/z_rcp.h"

#define FLAGS 0

void BgSpot17Bakudankabe_Init(Actor* thisx, GlobalContext* globalCtx);
void BgSpot17Bakudankabe_Reset(Actor* pthisx, GlobalContext* globalCtx);
void BgSpot17Bakudankabe_Destroy(Actor* thisx, GlobalContext* globalCtx);
void BgSpot17Bakudankabe_Update(Actor* thisx, GlobalContext* globalCtx);
void BgSpot17Bakudankabe_Draw(Actor* thisx, GlobalContext* globalCtx);

ActorInit Bg_Spot17_Bakudankabe_InitVars = {
    ACTOR_BG_SPOT17_BAKUDANKABE,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_SPOT17_OBJ,
    sizeof(BgSpot17Bakudankabe),
    (ActorFunc)BgSpot17Bakudankabe_Init,
    (ActorFunc)BgSpot17Bakudankabe_Destroy,
    (ActorFunc)BgSpot17Bakudankabe_Update,
    (ActorFunc)BgSpot17Bakudankabe_Draw,
    (ActorFunc)BgSpot17Bakudankabe_Reset,
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 3000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 500, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 1000, ICHAIN_STOP),
};

void func_808B6BC0(BgSpot17Bakudankabe* pthis, GlobalContext* globalCtx) {
    s32 pad[2];
    s32 i;
    Vec3f burstDepthY;
    Vec3f burstDepthX;
    f32 sinY;
    f32 cosY;

    sinY = Math_SinS(pthis->dyna.actor.shape.rot.y);
    cosY = Math_CosS(pthis->dyna.actor.shape.rot.y);

    burstDepthX.z = 0.0f;
    burstDepthX.x = 0.0f;

    for (i = 0; i < 20; i++) {
        s16 gravityInfluence;
        s16 scale;
        f32 temp1;
        f32 temp2;
        s32 rotationSpeed;

        temp1 = (Rand_ZeroOne() - 0.5f) * 140.0f;
        temp2 = (Rand_ZeroOne() - 0.5f) * 20.0f;

        burstDepthY.x = pthis->dyna.actor.world.pos.x + temp2 * sinY + (temp1 * cosY);
        burstDepthY.y = pthis->dyna.actor.world.pos.y + 30.0f + (i * 6.5f);
        burstDepthY.z = pthis->dyna.actor.world.pos.z + temp2 * cosY - (temp1 * sinY);

        burstDepthX.y = (Rand_ZeroOne() - 0.2f) * 12.0f;
        scale = Rand_ZeroOne() * 55.0f + 8.0f;

        if (scale < 20) {
            gravityInfluence = -300;
        } else if (scale < 35) {
            gravityInfluence = -360;
        } else {
            gravityInfluence = -420;
        }

        if (Rand_ZeroOne() < 0.4f) {
            rotationSpeed = 65;
        } else {
            rotationSpeed = 33;
        }
        EffectSsKakera_Spawn(globalCtx, &burstDepthY, &burstDepthX, &burstDepthY, gravityInfluence, rotationSpeed, 0x1E,
                             4, 0, scale, 1, 3, 80, KAKERA_COLOR_NONE, OBJECT_GAMEPLAY_FIELD_KEEP, gFieldKakeraDL);
    }
    Math_Vec3f_Copy(&burstDepthY, &pthis->dyna.actor.world.pos);
    func_80033480(globalCtx, &burstDepthY, 60.0f, 4, 110, 160, 1);
    burstDepthY.y += 40.0f;
    func_80033480(globalCtx, &burstDepthY, 60.0f, 4, 120, 160, 1);
    burstDepthY.y += 40.0f;
    func_80033480(globalCtx, &burstDepthY, 60.0f, 4, 110, 160, 1);
}

void BgSpot17Bakudankabe_Init(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot17Bakudankabe* pthis = (BgSpot17Bakudankabe*)thisx;
    s32 pad;
    CollisionHeader* colHeader = NULL;

    DynaPolyActor_Init(&pthis->dyna, DPM_UNK);
    if (Flags_GetSwitch(globalCtx, (pthis->dyna.actor.params & 0x3F))) {
        Actor_Kill(&pthis->dyna.actor);
        return;
    }

    CollisionHeader_GetVirtual(&gCraterBombableWallCol, &colHeader);
    pthis->dyna.bgId = DynaPoly_SetBgActor(globalCtx, &globalCtx->colCtx.dyna, &pthis->dyna.actor, colHeader);
    Actor_ProcessInitChain(&pthis->dyna.actor, sInitChain);
}

void BgSpot17Bakudankabe_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot17Bakudankabe* pthis = (BgSpot17Bakudankabe*)thisx;
    DynaPoly_DeleteBgActor(globalCtx, &globalCtx->colCtx.dyna, pthis->dyna.bgId);
}

void BgSpot17Bakudankabe_Update(Actor* thisx, GlobalContext* globalCtx) {
    BgSpot17Bakudankabe* pthis = (BgSpot17Bakudankabe*)thisx;
    if (pthis->dyna.actor.xzDistToPlayer < 650.0f && func_80033684(globalCtx, &pthis->dyna.actor) != NULL) {
        func_808B6BC0(pthis, globalCtx);
        Flags_SetSwitch(globalCtx, (pthis->dyna.actor.params & 0x3F));
        Audio_PlaySoundAtPosition(globalCtx, &pthis->dyna.actor.world.pos, 40, NA_SE_EV_WALL_BROKEN);
        Common_PlaySfx(NA_SE_SY_CORRECT_CHIME);
        Actor_Kill(&pthis->dyna.actor);
    }
}

void BgSpot17Bakudankabe_Draw(Actor* thisx, GlobalContext* globalCtx) {
    s32 pad;
    s8 r = coss(globalCtx->gameplayFrames * 1500) >> 8;
    s8 g = coss(globalCtx->gameplayFrames * 1500) >> 8;

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_spot17_bakudankabe.c", 269);

    func_80093D18(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_spot17_bakudankabe.c", 273),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    r = (r >> 1) + 0xC0;
    g = (g >> 1) + 0xC0;

    gDPSetEnvColor(POLY_OPA_DISP++, r, g, 255, 128);

    gSPDisplayList(POLY_OPA_DISP++, gCraterBombableWallDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_spot17_bakudankabe.c", 283);

    OPEN_DISPS(globalCtx->state.gfxCtx, "../z_bg_spot17_bakudankabe.c", 286);

    func_80093D84(globalCtx->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(globalCtx->state.gfxCtx, "../z_bg_spot17_bakudankabe.c", 290),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, gCraterBombableWallCracksDL);

    CLOSE_DISPS(globalCtx->state.gfxCtx, "../z_bg_spot17_bakudankabe.c", 295);
}

void BgSpot17Bakudankabe_Reset(Actor* pthisx, GlobalContext* globalCtx) {
    Bg_Spot17_Bakudankabe_InitVars = {
        ACTOR_BG_SPOT17_BAKUDANKABE,
        ACTORCAT_BG,
        FLAGS,
        OBJECT_SPOT17_OBJ,
        sizeof(BgSpot17Bakudankabe),
        (ActorFunc)BgSpot17Bakudankabe_Init,
        (ActorFunc)BgSpot17Bakudankabe_Destroy,
        (ActorFunc)BgSpot17Bakudankabe_Update,
        (ActorFunc)BgSpot17Bakudankabe_Draw,
        (ActorFunc)BgSpot17Bakudankabe_Reset,
    };

}
