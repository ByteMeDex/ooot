#define INTERNAL_SRC_OVERLAYS_ACTORS_OVL_EN_RIVER_SOUND_Z_EN_RIVER_SOUND_C
#include "actor_common.h"
/*
 * File: z_en_river_sound.c
 * Overlay: ovl_En_River_Sound
 * Description: Ambient Sound Effects
 */

#include "z_en_river_sound.h"
#include "def/audio.h"
#include "def/z_actor.h"
#include "def/z_bgcheck.h"
#include "def/z_lib.h"

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_5)

void EnRiverSound_Init(Actor* thisx, GlobalContext* globalCtx);
void EnRiverSound_Reset(Actor* pthisx, GlobalContext* globalCtx);
void EnRiverSound_Destroy(Actor* thisx, GlobalContext* globalCtx);
void EnRiverSound_Update(Actor* thisx, GlobalContext* globalCtx);
void EnRiverSound_Draw(Actor* thisx, GlobalContext* globalCtx);

static s16 soundEffects_23[] = {
    0,
    NA_SE_EV_WATER_WALL - SFX_FLAG,
    NA_SE_EV_MAGMA_LEVEL - SFX_FLAG,
    NA_SE_EV_WATER_WALL_BIG - SFX_FLAG,
    0,
    0,
    NA_SE_EV_MAGMA_LEVEL_M - SFX_FLAG,
    NA_SE_EV_MAGMA_LEVEL_L - SFX_FLAG,
    NA_SE_EV_WATERDROP - SFX_FLAG,
    NA_SE_EV_FOUNTAIN - SFX_FLAG,
    NA_SE_EV_CROWD - SFX_FLAG,
    0,
    NA_SE_EV_SARIA_MELODY - SFX_FLAG,
    0,
    NA_SE_EV_SAND_STORM - SFX_FLAG,
    NA_SE_EV_WATER_BUBBLE - SFX_FLAG,
    NA_SE_EV_KENJA_ENVIROMENT_0 - SFX_FLAG,
    NA_SE_EV_KENJA_ENVIROMENT_1 - SFX_FLAG,
    NA_SE_EV_EARTHQUAKE - SFX_FLAG,
    0,
    NA_SE_EV_TORCH - SFX_FLAG,
    NA_SE_EV_COW_CRY_LV - SFX_FLAG,
};

static f32 soundPitch_23[] = { 0.7f, 1.0f, 1.4f };


ActorInit En_River_Sound_InitVars = {
    ACTOR_EN_RIVER_SOUND,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnRiverSound),
    (ActorFunc)EnRiverSound_Init,
    (ActorFunc)EnRiverSound_Destroy,
    (ActorFunc)EnRiverSound_Update,
    (ActorFunc)EnRiverSound_Draw,
    (ActorFunc)EnRiverSound_Reset,
};

void EnRiverSound_Init(Actor* thisx, GlobalContext* globalCtx) {
    EnRiverSound* pthis = (EnRiverSound*)thisx;

    pthis->playSound = 0;
    pthis->pathIndex = (pthis->actor.params >> 8) & 0xFF;
    pthis->actor.params = pthis->actor.params & 0xFF;

    if (pthis->actor.params >= RS_MAX) { // used for ganon and ganon_boss scenes
        func_800F4870(pthis->actor.params - RS_MAX);
        Actor_Kill(&pthis->actor);
    } else if (pthis->actor.params == RS_UNK_F7) {
        func_800F6FB4(4);
        Actor_Kill(&pthis->actor);
    } else if (pthis->actor.params == RS_SARIAS_SONG) {
        if (!CHECK_QUEST_ITEM(QUEST_SONG_LULLABY) || CHECK_QUEST_ITEM(QUEST_SONG_SARIA)) {
            Actor_Kill(&pthis->actor);
        }
    }
}

void EnRiverSound_Destroy(Actor* thisx, GlobalContext* globalCtx) {
    EnRiverSound* pthis = (EnRiverSound*)thisx;

    if (pthis->actor.params == RS_SARIAS_SONG) {
        Audio_ClearSariaBgmAtPos(&pthis->actor.projectedPos);
    } else if (pthis->actor.params == RS_UNK_13) {
        Audio_ClearSariaBgm2();
    }
}

s32 func_80AE6A54(Vec3f* arg0, Vec3f* arg1, Vec3f* arg2, Vec3f* arg3) {
    Vec3f vec[3];
    f32 temp;

    vec[0].x = arg0->x - arg2->x;
    vec[0].y = arg0->y - arg2->y;
    vec[0].z = arg0->z - arg2->z;

    vec[1].x = arg1->x - arg2->x;
    vec[1].y = arg1->y - arg2->y;
    vec[1].z = arg1->z - arg2->z;

    vec[2].x = vec[1].x - vec[0].x;
    vec[2].y = vec[1].y - vec[0].y;
    vec[2].z = vec[1].z - vec[0].z;

    temp = DOTXYZ(vec[2], vec[0]);

    if ((DOTXYZ(vec[2], vec[1]) * temp) < 0.0f) {
        temp = -temp / (SQ(vec[2].x) + SQ(vec[2].y) + SQ(vec[2].z));

        arg3->x = (vec[2].x * temp) + arg0->x;
        arg3->y = (vec[2].y * temp) + arg0->y;
        arg3->z = (vec[2].z * temp) + arg0->z;

        return true;
    }

    return false;
}

/**
 * Writes the position along the river path to `soundPos` based on the `hearPos`, which is usually the position of the
 * player.
 * Returns true if the distance between the `hearPos` and `soundPos` is less than 10000, false if not.
 */
s32 EnRiverSound_GetSoundPos(Vec3s* points, s32 numPoints, Vec3f* hearPos, Vec3f* soundPos) {
    s32 i;
    s32 pointIdx;
    s32 sp78[2] = { 0, 0 };
    Vec3f pointLoc;
    Vec3f sp60;
    Vec3f sp54;
    Vec3f vec;
    f32 pointDist = 10000.0f;
    Vec3s* point;

    for (i = 0; i < numPoints; i++) {
        f32 dist;

        vec.x = points[i].x;
        vec.y = points[i].y;
        vec.z = points[i].z;
        dist = Math_Vec3f_DistXYZ(hearPos, &vec);

        if (dist < pointDist) {
            pointDist = dist;
            pointIdx = i;
        }
    }

    if (pointDist >= 10000.0f) {
        return false;
    }

    point = &points[pointIdx];
    pointLoc.x = point->x;
    pointLoc.y = point->y;
    pointLoc.z = point->z;

    if (pointIdx != 0) {
        vec.x = point[-1].x;
        vec.y = point[-1].y;
        vec.z = point[-1].z;
        sp78[0] = func_80AE6A54(&vec, &pointLoc, hearPos, &sp54);
    }

    if (pointIdx + 1 != numPoints) {
        vec.x = point[1].x;
        vec.y = point[1].y;
        vec.z = point[1].z;
        sp78[1] = func_80AE6A54(&pointLoc, &vec, hearPos, &sp60);
    }

    if (sp78[0] && sp78[1]) {
        if (!func_80AE6A54(&sp54, &sp60, hearPos, soundPos)) {
            soundPos->x = (sp54.x + sp60.x) * 0.5f;
            soundPos->y = (sp54.y + sp60.y) * 0.5f;
            soundPos->z = (sp54.z + sp60.z) * 0.5f;
        }
    } else if (sp78[0]) {
        soundPos->x = sp54.x;
        soundPos->y = sp54.y;
        soundPos->z = sp54.z;
    } else if (sp78[1]) {
        soundPos->x = sp60.x;
        soundPos->y = sp60.y;
        soundPos->z = sp60.z;
    } else {
        soundPos->x = pointLoc.x;
        soundPos->y = pointLoc.y;
        soundPos->z = pointLoc.z;
    }

    return true;
}

void EnRiverSound_Update(Actor* thisx, GlobalContext* globalCtx) {
    Path* path;
    Vec3f* pos;
    Player* player = GET_PLAYER(globalCtx);
    EnRiverSound* pthis = (EnRiverSound*)thisx;
    s32 sp34;

    if ((thisx->params == RS_UNK_0) || (thisx->params == RS_UNK_4) || (thisx->params == RS_UNK_5)) {
        path = &globalCtx->setupPathList[pthis->pathIndex];
        pos = &thisx->world.pos;

        if (EnRiverSound_GetSoundPos(SEGMENTED_TO_VIRTUAL(path->points), path->count, &player->actor.world.pos, pos)) {
            if (BgCheck_EntityRaycastFloor4(&globalCtx->colCtx, &thisx->floorPoly, &sp34, thisx, pos) !=
                BGCHECK_Y_MIN) {
                // Get the sound volume pitch based on the speed of the river current under the actor
                pthis->soundPitchIndex = SurfaceType_GetConveyorSpeed(&globalCtx->colCtx, thisx->floorPoly, sp34);
            } else {
                pthis->soundPitchIndex = 0;
            }

            if (pthis->soundPitchIndex == 0) {
                if (thisx->params == RS_UNK_4) {
                    pthis->soundPitchIndex = 0;
                } else if (thisx->params == RS_UNK_0) {
                    pthis->soundPitchIndex = 1;
                } else {
                    pthis->soundPitchIndex = 2;
                }
            } else {
                pthis->soundPitchIndex--;
                pthis->soundPitchIndex = CLAMP_MAX(pthis->soundPitchIndex, 2);
            }
        }
    } else if ((thisx->params == RS_UNK_13) || (thisx->params == RS_UNK_19)) {
        func_8002DBD0(&player->actor, &thisx->home.pos, &thisx->world.pos);
    } else if (globalCtx->sceneNum == SCENE_DDAN_BOSS && Flags_GetClear(globalCtx, thisx->room)) {
        Actor_Kill(thisx);
    }
}

void EnRiverSound_Draw(Actor* thisx, GlobalContext* globalCtx) {
    EnRiverSound* pthis = (EnRiverSound*)thisx;

    if (!(pthis->playSound)) {
        pthis->playSound = true;
    } else if ((pthis->actor.params == RS_UNK_0) || (pthis->actor.params == RS_UNK_4) ||
               (pthis->actor.params == RS_UNK_5)) {
        Audio_PlaySoundRiver(&pthis->actor.projectedPos, soundPitch_23[pthis->soundPitchIndex]);
    } else if (pthis->actor.params == RS_UNK_11) {
        Audio_RequestBgmVolumeChange(90);
    } else if (pthis->actor.params == RS_SARIAS_SONG) {
        func_800F4E30(&pthis->actor.projectedPos, pthis->actor.xzDistToPlayer);
    } else if (pthis->actor.params == RS_UNK_13) {
        Audio_PlaySariaBgm(&pthis->actor.home.pos, NA_BGM_SARIA_THEME, 1000);
    } else if (pthis->actor.params == RS_UNK_19) {
        Audio_PlaySariaBgm(&pthis->actor.home.pos, NA_BGM_GREAT_FAIRY, 800);
    } else if ((pthis->actor.params == RS_SANDSTORM) || (pthis->actor.params == RS_CHAMBER_OF_SAGES_1) ||
               (pthis->actor.params == RS_CHAMBER_OF_SAGES_2) || (pthis->actor.params == RS_RUMBLING)) {
        Common_PlaySfx2(soundEffects_23[pthis->actor.params]);
    } else {
        Audio_PlayActorSound2(&pthis->actor, soundEffects_23[pthis->actor.params]);
    }
}

void EnRiverSound_Reset(Actor* pthisx, GlobalContext* globalCtx) {
    En_River_Sound_InitVars = {
        ACTOR_EN_RIVER_SOUND,
        ACTORCAT_BG,
        FLAGS,
        OBJECT_GAMEPLAY_KEEP,
        sizeof(EnRiverSound),
        (ActorFunc)EnRiverSound_Init,
        (ActorFunc)EnRiverSound_Destroy,
        (ActorFunc)EnRiverSound_Update,
        (ActorFunc)EnRiverSound_Draw,
        (ActorFunc)EnRiverSound_Reset,
    };

}
