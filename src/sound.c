

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>
#include "sound.h"
#include "WavLib.h"

bool soundEnabled;

FSOUND_SAMPLE SFX[NUMSFX];

int SFXMasterVolume = 128;
int MasterVolume = 128;
int frequency=11025; //default
int channel;

int isMusicPlaying = 0;
Mix_Music * lastPlayed;
int musicPaused = 0;

void soundInit()
{
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		SFX[i].used=false;
	}

	if(csndInit()==0)soundEnabled=true;
	else soundEnabled=false;
}

void soundClose()
{
	int i;

	CSND_SetPlayState(15, 0);//Stop music audio playback.
	CSND_UpdateInfo(0);

	for(i=0;i<NUMSFX;i++)
	{
		if(SFX[i].used)
		{
			if(SFX[i].data)
			{
				linearFree(SFX[i].data);
				SFX[i].data=NULL;
			}
			SFX[i].used=false;
		}
	}
	if(soundEnabled)csndExit();
}

FILE* openFile(const char* fn, const char* mode)
{
	if(!fn || !mode)return NULL;
	return fopen(fn, mode);
}

void* bufferizeFile(const char* filename, u32* size, bool binary, bool linear)
{

	u8 *WAVData, *buffer;
	long      WAVElements;
	WAVInfo_t WaveFile;
	WaveFile=WAV_Open(filename);
	if (!WaveFile) 
		return NULL;
	WAVData= (u8*) WAV_GetData(WaveFile,&WAVElements);
	
	if(!WAVData)
	{
		WAV_Free (WaveFile);
		return NULL;
	}
	// gets only first channel
	u8 step = WAV_Channels (WaveFile);
	int samplesize=WAV_BitsPerSample(WaveFile);
 
	if (samplesize==8)
	{
		if(linear)buffer=(u8*)linearMemAlign(WAVElements, 0x80);
		else buffer=(u8*)malloc(WAVElements);
		if(size)*size=WAVElements;
	
		if(!buffer)
		{
			free(WAVData);
			WAV_Free (WaveFile);
			return NULL;
		}
		long count;
		for(count=0; count < WAVElements; count++) 
			buffer[count]=WAVData[count*step];
		free(WAVData);
		WAV_Free (WaveFile);
		return buffer;
	} else if (samplesize==16) {
		if(linear)buffer=(u8*)linearMemAlign(WAVElements*2, 0x80);
		else buffer=(u8*)malloc(WAVElements*2);
		if(size)*size=WAVElements;
	
		if(!buffer)
		{
			free(WAVData);
			WAV_Free (WaveFile);
			return NULL;
		}
		long count;
		for(count=0; count < WAVElements; count++)
			((u16*)buffer)[count]=((u16*)WAVData)[count*step];
		free(WAVData);
		WAV_Free (WaveFile);
		return buffer;
	} else {
		free(WAVData);
		WAV_Free (WaveFile);
		return NULL;
	}
}


int FSOUND_Init(u32 freq, u32 bps, u32 unkn)
{

	frequency = freq;
	
	return soundEnabled;
}

void initSFX(FMUSIC_MODULE* s)
{
	if(!s)return;

	s->data=NULL;
	s->size=0;
	s->used=true;
	s->loop=false;
}

void loadSFX(FMUSIC_MODULE* s, const char* filename, u32 format)
{
	if(!s)return;

	initSFX(s);

	s->data=(u8*) bufferizeFile(filename, &s->size, true, true);
	s->format=format;
}

int FSOUND_GetSFXMasterVolume()
{
	return SFXMasterVolume;
}

int FMUSIC_GetMasterVolume(FMUSIC_MODULE* s)
{
	return MasterVolume;
}

void FMUSIC_SetMasterVolume(FMUSIC_MODULE* s, u8 volume)
{
	MasterVolume = volume;
}

void FSOUND_SetSFXMasterVolume(u8 volson)
{
	SFXMasterVolume = volson;
}

void FSOUND_PlaySound(int chl,FSOUND_SAMPLE* s)
{
	if(!s || !s->used || !s->data || !soundEnabled || SFXMasterVolume == 0)return;

	channel++;
	channel%=7;

	csndPlaySound(channel+8, s->format, frequency, (float)SFXMasterVolume/MIX_MAX_VOLUME, 0.0, (u32*)s->data, (u32*)s->data, s->size);
}

void FMUSIC_StopSong(FMUSIC_MODULE* s)
{
	CSND_SetPlayState(15, 0);//Stop music audio playback.
	csndExecCmds(0);
}

void FMUSIC_PlaySong(FMUSIC_MODULE* s)
{
	int flag;
	if(!s || !s->used || !s->data || !soundEnabled || MasterVolume == 0)return;
	flag = s->format;
	if(s->loop) flag |= SOUND_REPEAT;
	csndPlaySound(15, flag, 11025, (float)MasterVolume/MIX_MAX_VOLUME, 0.0, (u32*)s->data, (u32*)s->data, s->size);
}


FSOUND_SAMPLE* FSOUND_Sample_Load(int flag, const char * f,int a, int b, int c)
{
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		if(!SFX[i].used)
		{
			loadSFX(&SFX[i], f, SOUND_FORMAT_16BIT);

			if(!SFX[i].data)return NULL;
			SFX[i].used = true;
			SFX[i].loop=false;
			return &SFX[i];
		}
	}
	return NULL;
}

FMUSIC_MODULE* FMUSIC_LoadSong(const char * f)
{
	int i;
	for(i=0;i<NUMSFX;i++)
	{
		if(!SFX[i].used)
		{
			loadSFX(&SFX[i], f, SOUND_FORMAT_16BIT);
			
			if(!SFX[i].data) return NULL;
			SFX[i].used = true;
			SFX[i].loop=false;
			return &SFX[i];
		}
	}
	return NULL;
}

void FSOUND_Close()
{
	return;
}

void Mix_CloseAudio()
{
	return;
}


void FMUSIC_SetLooping(FMUSIC_MODULE* s, bool flag)
{
	if (s)
		s->loop=flag;
}

void FSOUND_Sample_Free(FSOUND_SAMPLE* s)
{
	if(s) {
		if (s->data)
			linearFree(s->data);
		s->size=0;
		s->used=false;
		s->loop=false;
	}
}


void FMUSIC_FreeSong(FMUSIC_MODULE* s)
{
	if(s) {
		if (s->data)
			linearFree(s->data);
		s->size=0;
		s->used=false;
		s->loop=false;
	}
}

void Mix_FreeChunk(Mix_Chunk* s)
{
	if(s) {
		if (s->data)
			linearFree(s->data);
		s->size=0;
		s->used=false;
		s->loop=false;
	}
}

void Mix_FreeMusic(Mix_Music* s)
{
	if(s) {
		if (s->data)
			linearFree(s->data);
		s->size=0;
		s->used=false;
		s->loop=false;
	}
}

int Mix_PlayChannel(int channel, Mix_Chunk *s, int loops)
{
	if(!s || !s->used || !s->data || !soundEnabled || SFXMasterVolume == 0) return -1;

	channel++;
	channel%=7;
	if(loops) s->format |= SOUND_REPEAT;
	else s->format &= ~SOUND_REPEAT;

	csndPlaySound(channel+8, s->format, frequency, (float)SFXMasterVolume/MIX_MAX_VOLUME, 0.0, (u32*)s->data, (u32*)s->data, s->size);
	
	return 0;
}

int Mix_PlayMusic(Mix_Music * s , int loops )
{
	int flag;
	if(!s || !s->used || !s->data || !soundEnabled || MasterVolume == 0) return -1;
	lastPlayed = s;
	flag = s->format;
	if (loops) flag |= SOUND_REPEAT;
	csndPlaySound(15, flag, 11025, (float)MasterVolume/MIX_MAX_VOLUME, 0.0, (u32*)s->data, (u32*)s->data, s->size);
	isMusicPlaying = 1;
	return 0;
}

int Mix_PlayingMusic()
{
	return isMusicPlaying;
}

int Mix_OpenAudio( int audio_rate, u16 audio_format, int audio_channels, int bufsize ){
	return FSOUND_Init(audio_rate, 0, 0);
}

void Mix_VolumeMusic( int volume ){
	MasterVolume = volume;
}

void Mix_VolumeChunk(Mix_Chunk *chunk, int volume){
	SFXMasterVolume = volume;
}

Mix_Chunk * Mix_LoadWAV(const char * f) {
	return FSOUND_Sample_Load(0, f,0, 0, 0);
}

Mix_Music * Mix_LoadMUS(const char *f){
	return FMUSIC_LoadSong(f);
}

void Mix_HaltMusic()
{
	
	CSND_SetPlayState(15, 0);//Stop music audio playback.
	CSND_UpdateInfo(0);
}

void Mix_PauseMusic()
{
	
	CSND_SetPlayState(15, 0);//Stop music audio playback.
	CSND_UpdateInfo(0);
	musicPaused = 1;
}

int Mix_PausedMusic()
{
	return musicPaused;
}

void Mix_ResumeMusic()
{
	Mix_PlayMusic(lastPlayed , 1 );
	musicPaused=0;
}


const char * Mix_GetError(){
	return SDL_GetError();
}
