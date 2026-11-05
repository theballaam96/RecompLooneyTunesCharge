#include "modding.h"
#include "ultra64.h"
#include "recompconfig.h"
#include "recompdata.h"
#include "recompui.h"
#include "recompui_event_structs.h"
#include "rt64_extended_gbi.h"
#include "libaudio.h"
#include "enums.h"
#include "common_structs.h"

typedef struct NewAudioSample NewAudioSample;
struct NewAudioSample {
    u8 *sample;
    f32 sample_length; // In seconds
    u32 sample_size; // in bytes
    u8 bank; // 0 = OST, 1 = SFX
    u8 inst;
    u16 snd;
    u32 base_start; // DO NOT PRE-POPULATE
    u32 base_end; // DO NOT PRE-POPULATE
    NewAudioSample *prev; // DO NOT PRE-POPULATE
    NewAudioSample *next; // DO NOT PRE-POPULATE
};
RECOMP_IMPORT("audio_tools", void ReplaceSample(NewAudioSample *smp));

extern u8 charge_up[];
extern u32 charge_up_size;
extern u8 release[];
extern u32 release_size;
extern s32 *D_global_asm_807FBB50;

NewAudioSample charge_up_beta_sound = {
    .sample = &charge_up[0],
    .sample_length = 0.699f,
    .sample_size = 0,
    .bank = 1,
    .inst = 0,
    .snd = 258,
};

NewAudioSample release_beta_sound = {
    .sample = &release[0],
    .sample_length = 0.7f,
    .sample_size = 0,
    .bank = 1,
    .inst = 0,
    .snd = 408,
};

RECOMP_CALLBACK("*", recomp_on_init)
void chargeNoiseInit(void) {
    // Initialize the charge noise
    charge_up_beta_sound.sample_size = charge_up_size;
    release_beta_sound.sample_size = release_size;
    ReplaceSample(&charge_up_beta_sound);
    ReplaceSample(&release_beta_sound);
}

RECOMP_CALLBACK("*", dk64recomp_every_frame)
void chargeNoiseAnimWipe(void) {
    // Get rid of interfering sound calls
    u8 *offset;
    s32 i;

    if (D_global_asm_807FBB50) {
        offset = (u8 *)((s32)D_global_asm_807FBB50 + D_global_asm_807FBB50[280]);
        offset[0xCF] = 1;
        offset[0xD0] = 0x99;
        offset[0xD4] = 0; // Disable pitch variance
    }
}