#include "audio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

PHL_Sound loadWav(char* fname, int loop);

void PHL_AudioInit()
{
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
	ndspSetOutputCount(1);
}

void PHL_AudioClose()
{
	ndspExit();
}

PHL_Sound PHL_LoadSound(char* fname, int loop)
{
	return loadWav(fname,loop);
}

PHL_Sound loadWav(char* fname, int loop)
{
	PHL_Sound snd;
	
	
	FILE* f;
	if ( (f = fopen(fname, "rb")) )
	{
		//Check for valid fileName
		u32 sig;
		fread(&sig, 4, 1, f);
		
		if (sig == 0x46464952) //RIFF
		{
			u32 chunkSize;
			u32 format;
			
			fread(&chunkSize, 4, 1, f);
			fread(&format, 4, 1, f);
			
			if (format == 0x45564157) //WAVE
			{
				u32 subchunk1ID;
				fread(&subchunk1ID, 4, 1, f);
				
				if (subchunk1ID == 0x20746D66) //fmt
				{
					u32 subchunk1Size;
					u16 audioFormat;
					u16 numChannels;
					u32 sampleRate;
					u32 byteRate;
					u16 blockAlign;
					u16 bitsPerSample;
					
					fread(&subchunk1Size, 4, 1, f);
					fread(&audioFormat, 2, 1, f);
					fread(&numChannels, 2, 1, f);
					fread(&sampleRate, 4, 1, f);
					fread(&byteRate, 4, 1, f);
					fread(&blockAlign, 2, 1, f);
					fread(&bitsPerSample, 2, 1, f);
					
					//Search for 'data'
					for (int i = 0; i < 100; i++) {
						u8 c;
						fread(&c, 1, 1, f);
						if (c == 0x64) { //'d'
							fread(&c, 1, 1, f);
							if (c == 0x61) { //'a'
								fread(&c, 1, 1, f);
								if (c == 0x74) { //'t'
									fread(&c, 1, 1, f);
									if (c == 0x61) { //'a'
										i = 100;
									}
								}
							}
						}						
					}
					
					u32 subchunk2Size;
					fread(&subchunk2Size, 4, 1, f);
					
					snd.numChannels = numChannels;
					
					if(bitsPerSample == 8)
					{
						snd.ndspFormat = (numChannels == 1) ?
							NDSP_FORMAT_MONO_PCM8 :
							NDSP_FORMAT_STEREO_PCM8;
					}
					else
					{
						snd.ndspFormat = (numChannels == 1) ?
							NDSP_FORMAT_MONO_PCM16 :
							NDSP_FORMAT_STEREO_PCM16;
					}
					
					snd.sampleRate = sampleRate;
					snd.dataSize = subchunk2Size;
					snd.bitsPerSample = bitsPerSample;
					
					snd.data = (u8*)(linearAlloc(subchunk2Size));					
					fread(snd.data, 1, subchunk2Size, f);
				}
			}
		}
		
		fclose(f);
	}
	
	
	return snd;
}

void PHL_PlaySound(PHL_Sound snd, int channel)
{
	if (snd.data != NULL) {
		ndspChnReset(channel);
		ndspChnSetInterp(channel, NDSP_INTERP_NONE);
		ndspChnSetRate(channel, (float)snd.sampleRate);
		ndspChnSetFormat(channel, snd.ndspFormat);
		
		
		memset(&waveBuf[channel], 0, sizeof(ndspWaveBuf));
		waveBuf[channel].data_vaddr = (u32)(snd.data);
		waveBuf[channel].nsamples = snd.dataSize / (snd.bitsPerSample >> 3) / snd.numChannels;
		waveBuf[channel].looping = snd.loop;
		waveBuf[channel].status = NDSP_WBUF_FREE;
		
		DSP_FlushDataCache(snd.data, snd.dataSize);
		ndspChnWaveBufAdd(channel, &waveBuf[channel]);
	}
}


void PHL_StopSound(PHL_Sound snd, int channel)
{
	ndspChnWaveBufClear(channel);
}

void PHL_FreeSound(PHL_Sound snd)
{
	if (snd.data != NULL) {
		linearFree(snd.data);
		snd.data = NULL;
	}
}