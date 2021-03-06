#include <genesis.h>

#include "ready.h"

#include "sound.h"

u16 readyFrames;

void readyLoop(u16 * palette){
    switch(readyState){
        case READYENTRY:
            VDP_setPaletteColor(15, 0x000);
            VDP_drawText("Ready?", 16, 13);
            palette[15] = 0xFFF;
            VDP_fadeIn(15, 15, palette, READYFADE, TRUE);
            XGM_stopPlay();
            XGM_setPCM(BOOM_ID, boom, sizeof(boom));
            XGM_startPlayPCM(BOOM_ID, 0, SOUND_PCM_CH2);
            readyState = READY_FADE_IN;
            readyFrames = READYFADE;
        break;
        case READY_FADE_IN:
            if(VDP_isDoingFade() && readyFrames--) {
                return;
            }
            else {
                VDP_setPaletteColor(15, 0xFFF);
                readyState = READYLOOP;
                readyFrames = READYFRAMES;
            }
        break;
        case READYLOOP:
            if(readyFrames--) return;
            else {
                VDP_fadeOut(15,15,READYFADE,TRUE);
                readyState = READY_FADE_OUT;
                readyFrames = READYFADE;
            }
        break;
        case READY_FADE_OUT:
            if(VDP_isDoingFade() && readyFrames--) {
                return;
            }
            else {
                readyState = READYEXIT;
            }
        break;
        case READYEXIT:
            VDP_clearPlan(PLAN_A, TRUE);
            mainState = GAMEPLAY;
            readyState = READYENTRY;
        break;
    }
}