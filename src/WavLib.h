/* WavLib.h
 * COMS30123
 * Michael McTernan, mm7323@bris.ac.uk
 * 17/02/00
 */

#ifndef WAVLIB_HEADER
#define WAVLIB_HEADER



typedef struct WAVInfo * WAVInfo_t;

typedef struct {
  long Name;
  long Position;
} cuepoint_t;

WAVInfo_t  WAV_New          (int Bits,int Channels,int SampleRate);
WAVInfo_t  WAV_Open         (const char *File);
void      *WAV_GetData      (WAVInfo_t W,long *Elements);
void       WAV_AddCuePoint  (WAVInfo_t W,long Position);
int        WAV_GetCueCount  (WAVInfo_t W);
cuepoint_t WAV_CueIterate   (WAVInfo_t W);
void       WAV_ResetIterator(WAVInfo_t W);
void       WAV_Write        (WAVInfo_t W,char *Filename);
int        WAV_Channels     (WAVInfo_t W);
short      WAV_BitsPerSample(WAVInfo_t W);
long       WAV_SampleFreq   (WAVInfo_t W);
void       WAV_SetData      (WAVInfo_t W,void *Data,long Elements);
void       WAV_Free         (WAVInfo_t W);

#endif
