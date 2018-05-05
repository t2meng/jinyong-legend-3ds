#ifndef AUDIO_H
#define AUDIO_H

#include <3ds.h>

//#include "dr_wav.h"

typedef struct{
	u32 sampleRate;
	u32 dataSize;	
	u16 bitsPerSample;
	u16 ndspFormat;
	u16 numChannels;
	u8 loop;
	u8* data;
} PHL_Sound;

ndspWaveBuf waveBuf[6]; //One for each channel?

void PHL_AudioInit();
void PHL_AudioClose();

PHL_Sound PHL_LoadSound(char* fname, int loop);

void PHL_PlaySound(PHL_Sound snd, int channel);

void PHL_StopSound(PHL_Sound snd, int channel);

void PHL_FreeSound(PHL_Sound snd);

#endif