#ifndef __SOUND_H__
#define __SOUND_H__


#include <stdlib.h>
#include <3ds.h>

//******************** Audio

#define FSOUND_FREE 1
#define NUMSFX (45)

#define AUDIO_S16 SOUND_FORMAT_16BIT
#define MIX_MAX_VOLUME 1
#define MIX_DEFAULT_FORMAT SOUND_FORMAT_16BIT



typedef struct FSOUND_SAMPLE
{
	u8* data;
	u32 size;
	u32 format;
	bool used;
	bool loop;
} FSOUND_SAMPLE;


typedef FSOUND_SAMPLE FMUSIC_MODULE;

typedef FSOUND_SAMPLE Mix_Chunk;
typedef FSOUND_SAMPLE Mix_Music;


void soundInit();
void soundClose();

int FSOUND_Init(u32 freq, u32 bpd, u32 unkn);

int FSOUND_GetSFXMasterVolume(); //
int FMUSIC_GetMasterVolume(FMUSIC_MODULE* s); 


void FMUSIC_SetMasterVolume(FMUSIC_MODULE* s, u8 volume);
void FMUSIC_FreeSong(FMUSIC_MODULE* s);
void FSOUND_SetSFXMasterVolume(u8 volson);

void initSFX(FSOUND_SAMPLE* s);
void loadSFX(FSOUND_SAMPLE* s, const char* filename, u32 format);

void FSOUND_PlaySound(int chl,FSOUND_SAMPLE* s);
void FMUSIC_StopSong(FMUSIC_MODULE* s);
void FMUSIC_SetLooping(FMUSIC_MODULE* s, bool flag);
void FMUSIC_PlaySong(FMUSIC_MODULE* s);

FSOUND_SAMPLE* FSOUND_Sample_Load(int flag, const char * f,int a, int b, int c);
FMUSIC_MODULE* FMUSIC_LoadSong(const char * f); 

Mix_Chunk * Mix_LoadWAV(const char * f);
Mix_Music * Mix_LoadMUS(const char *f);

int Mix_PlayChannel(int channel, Mix_Chunk *s, int loops);
int Mix_PlayMusic(Mix_Music * s , int loops );
int Mix_PlayingMusic(void);


void Mix_FreeChunk(Mix_Chunk* s);
void Mix_FreeMusic(Mix_Music* s);
void Mix_HaltMusic();


void FSOUND_Sample_Free(FSOUND_SAMPLE* s);

void FSOUND_Close();
void Mix_CloseAudio();

int Mix_OpenAudio( int audio_rate, u16 audio_format, int audio_channels, int bufsize );

void Mix_VolumeMusic( int vol );

const char * Mix_GetError();

int Mix_PausedMusic();
void Mix_PauseMusic();
void Mix_ResumeMusic();

#endif
