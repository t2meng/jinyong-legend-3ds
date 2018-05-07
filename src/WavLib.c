/* WavLib.c
 * Michael McTernan, mm7323@bris.ac.uk
 * 17/02/00
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WavLib.h"

#define Error(a) {fprintf(stderr,a);abort();}

/* RIFF Definition */
/* Taken from Assignment handout
 */
typedef struct WAVHeader{     
        char riff[4];         /* "RIFF" 4 bytes */ 
        long TotLen;          /* Total length 4 bytes */ 
        char wavefmt[8];      /* "WAVEfmt " 8 bytes */ 
        long Len;             /* Remaining length 4 bytes */ 
        short format_tag;     /* Tag (1 = PCM) 2 bytes */ 
        short channels;       /* Mono=1 Stereo=2 2 bytes */ 
        long SamplesPerSec;   /* No samples/sec 4 bytes */ 
        long AvgBytesPerSec;  /* Average bytes/sec 4 bytes */ 
        short BlockAlign;     /* Block align 2 bytes */ 
        short wBitsPerSample; /* 8 or 16 bit 2 bytes */ 
        char data[4];         /* "data" 4 bytes */ 
        long datalen;         /* Raw data length 4 bytes */ 

} WAVHeader_t;

/* The structure for a cuepoint */
/* Found on the web
 */
typedef struct cuepoint {
  long dwName;
  long dwPosition;
  char fccChunk[4];
  long dwChunkStart;
  long dwBlockStart;
  long dwSampleOffset;
  struct cuepoint *Next;  /* This doesn't get saved to the file! */
} wavcuepoint_t;

/* The type for the ADT */
struct WAVInfo {
  WAVHeader_t WAVHead;
  long        CueCount,Elements;
  wavcuepoint_t *CueList,*CueEnd,*CueIterator;
  void       *RawData;
};


/* Needed for sparcs */
static void FixEndianS(short *n);
static void FixEndianL(long *n);

/*WAV_New*/
/* Create a new WAV file
 */
WAVInfo_t WAV_New(int Bits,int Channels,int SampleRate) {
  struct WAVInfo *W;

  if((W=(struct WAVInfo*)calloc(sizeof(struct WAVInfo),1))==NULL)
    Error("calloc failed");
  
  /* Create the header block */
  memcpy(W->WAVHead.riff,"RIFF",4);
  memcpy(W->WAVHead.wavefmt,"WAVEfmt ",8);
  memcpy(W->WAVHead.data,"data",4);
  W->WAVHead.format_tag=1;
  W->WAVHead.wBitsPerSample=Bits;
  W->WAVHead.channels=Channels;
  W->WAVHead.BlockAlign=Bits/8;
  W->WAVHead.Len=16;
  W->WAVHead.SamplesPerSec=SampleRate;
  W->WAVHead.AvgBytesPerSec=SampleRate*Channels*(Bits/8);

  return W;
}


/*WAV_Open*/
/* Read a .WAV file
 */
WAVInfo_t WAV_Open(const char *File) {
  struct WAVInfo *W;
  FILE  *f;

  if((W=(struct WAVInfo*)calloc(sizeof(struct WAVInfo),1))==NULL)
    Error("calloc failed");
 
  /* Open the file for binary reads */
  if((f=fopen(File,"rb"))==NULL) 
    Error("fopen failed");

  /* Read the header block */
  if(fread(&W->WAVHead,1,sizeof(WAVHeader_t),f)!=sizeof(WAVHeader_t))
    Error("fread returned the wrong count");
 
  /* Validate the header block */
  if(strncmp(W->WAVHead.riff,"RIFF",4)!=0)
    Error("Not at valid wav file ('RIFF' missing or incorrect)");

  if(strncmp(W->WAVHead.wavefmt,"WAVEfmt",7)!=0)
     Error("Not at valid wav file ('WAVEfmt' missing or incorrect)");

  /* Fix the endians for the fields */
  FixEndianS(&W->WAVHead.format_tag); 
  FixEndianS(&W->WAVHead.channels);
  FixEndianS(&W->WAVHead.BlockAlign);   
  FixEndianS(&W->WAVHead.wBitsPerSample); 
  FixEndianL(&W->WAVHead.TotLen);
  FixEndianL(&W->WAVHead.Len);
  FixEndianL(&W->WAVHead.SamplesPerSec);
  FixEndianL(&W->WAVHead.AvgBytesPerSec);
  FixEndianL(&W->WAVHead.datalen);

  if(W->WAVHead.format_tag!=1)
     Error("Not Microsoft PCM encoded wav file");

  /* Dump some of the info */
  if(0) {
    printf("   Filename: %s\n",File);
    printf("    File is: %s\n",W->WAVHead.channels==1?"mono":"stereo");
    printf("Sample rate: %ldKHz\n",W->WAVHead.SamplesPerSec);
    printf("Sample bits: %d\n",W->WAVHead.wBitsPerSample);
  }

  /* Now check the data block */
  if(strncmp(W->WAVHead.data,"data",4)!=0)
     Error("Not at valid wav file ('data' missing or incorrect)");

  /* Now make and read in the data block */
  if((W->RawData=malloc(W->WAVHead.datalen))==NULL)
    Error("malloc failed");

  /* Now read all the data */
  if(fread(W->RawData,1,W->WAVHead.datalen,f)!=W->WAVHead.datalen)
    Error("fread on the data block failed");

  /* Check if there is cue information to read */
//  if(ftell(f)!=W->WAVHead.TotLen+8) {
//    char Header[4];
//    long Length;
//    wavcuepoint_t *Cue;
//    int  t;

    /* First read the header */
//    fread(Header,sizeof(char),4,f);

//    if(memcmp(Header,"cue ",4)!=0) 
//      Error("cue section expected");  
    
//    fread(&Length,sizeof(long),1,f);
//    fread(&W->CueCount,sizeof(long),1,f);

    /* Now read the entries */
//    for(t=0;t<W->CueCount;t++) {
      
//      if((Cue=(wavcuepoint_t *)malloc(sizeof(wavcuepoint_t)))==NULL)
//        Error("malloc failed");
      
//      fread(Cue,sizeof(wavcuepoint_t)-sizeof(wavcuepoint_t *),1,f);

      /* String onto end of linked list */
//      Cue->Next=NULL;

//      if(W->CueEnd!=NULL) 
//        W->CueEnd->Next=Cue,W->CueEnd=Cue;
//      else
//        W->CueEnd=W->CueList=Cue;

//    }
  
//  }
    

  /* Close the input file */
  fclose(f);

  return W;
}


/*WAV_GetData*/
/* Read a .WAV file
 */
void *WAV_GetData(struct WAVInfo *W,long *Elements) {
  long   t;
  void  *Data;

  /* Now convert the raw data into the return array */
  switch(W->WAVHead.wBitsPerSample) {
  case 8:
  {
    unsigned char *CData;

    if((Data=malloc(W->WAVHead.datalen))==NULL)
      Error("malloc failed");

    CData=(unsigned char *)Data;
      
    for(t=0;t<W->WAVHead.datalen;t++) {
      CData[t]=((unsigned char *)W->RawData)[t]-128;
    }
    *Elements=W->WAVHead.datalen;
    break;

  } 
  case 16:
  {
    short *SData;
    short *RawData=(short*)W->RawData;

    if((Data=malloc(W->WAVHead.datalen))==NULL)
      Error("malloc failed");

    SData=(short *)Data;

    for(t=0;t<W->WAVHead.datalen/sizeof(short);t++) {
      FixEndianS(&RawData[t]);
      SData[t]=RawData[t];
    }
    *Elements=W->WAVHead.datalen/2;
    break;
  
  }    
  default:
    Error("This sample file has an unsupported number of bits");
  }

  return Data;
}


/*WAV_AddCuePoint*/
/* Add a cue point to the list
 */
void WAV_AddCuePoint(struct WAVInfo *W,long Position) {
  wavcuepoint_t *c;

  if((c=(wavcuepoint_t *)malloc(sizeof(wavcuepoint_t)))==NULL)
    Error("malloc failed");

  memcpy(c->fccChunk,"data",4);

  c->dwName=++W->CueCount;
  c->dwChunkStart=0;     /* Has no meaning for PCMs */
  c->dwBlockStart=0;
  c->dwPosition=0;
  c->dwSampleOffset=Position; 
  c->Next=NULL;
    
  /* Add to the linked list */
  if(W->CueEnd!=NULL) {
    W->CueEnd->Next=c;
    W->CueEnd=c;
  }
  
  if(W->CueList==NULL) {
    W->CueList=c;
    W->CueEnd=c;
  }
  
}


/*WAV_GetCueCount*/
/* Return the number of cue points in the file
 */
int WAV_GetCueCount(WAVInfo_t W) {
  return W->CueCount;
}


/*WAV_CueIterate*/
/* Return each cue point sequentially.
 */
cuepoint_t WAV_CueIterate(WAVInfo_t W) {
  cuepoint_t NewCue;

  if(W->CueIterator==NULL) {
    NewCue.Name=-1;
    NewCue.Position=-1;
    W->CueIterator=W->CueList;
    return NewCue;
  }

  NewCue.Name=W->CueIterator->dwName;
  NewCue.Position=W->CueIterator->dwPosition;
  W->CueIterator=W->CueIterator->Next;
  
  return NewCue;
}


/*WAV_ResetIterator*/
/* Return each cue point sequentially.
 */
void WAV_ResetCueIterator(WAVInfo_t W) {
  W->CueIterator=W->CueList;
}


/*WAV_Write*/
/* Write the cue point list to the file
 */
void WAV_Write(WAVInfo_t W,char *Filename) {
  long Length;
  wavcuepoint_t *c;
  char Header[5]="cue ";
  FILE *f;

  /* Open the file for binary writes */
  if((f=fopen(Filename,"wb"))==NULL)
    Error("fopen failed");

  /* Write the WAV file header */
  if(fwrite(&W->WAVHead,1,sizeof(WAVHeader_t),f)!=sizeof(WAVHeader_t))
    Error("fwrite returned the wrong count");

  /* Now write all the data */
  if(fwrite(W->RawData,1,W->WAVHead.datalen,f)!=W->WAVHead.datalen)
    Error("fwrite on the data block failed");

  /* Check if there are cue points to write */
  if(W->CueCount>0) {

    /* Now write the cue points */
    /* First write the header */
    fwrite(Header,sizeof(char),4,f);
    Length=(sizeof(wavcuepoint_t)-sizeof(wavcuepoint_t *))*W->CueCount;
    Length+=2*sizeof(long);
    fwrite(&Length,sizeof(long),1,f);
    fwrite(&W->CueCount,sizeof(long),1,f);

    printf("Writing %ld cuepoints\n",W->CueCount);

    /* Now write the entries */
    c=W->CueList;

    while(c!=NULL) {
      fwrite(c,sizeof(wavcuepoint_t)-sizeof(wavcuepoint_t *),1,f);
      c=c->Next;
    }

  }

  /* Now update the total file length */
  W->WAVHead.TotLen=ftell(f)-sizeof(long)-4;
  rewind(f);
  if(fwrite(&W->WAVHead,1,sizeof(WAVHeader_t),f)!=sizeof(WAVHeader_t))
    Error("fwrite returned the wrong count");

  fclose(f);
}


/*WAV_Channels*/
/* Return number of channels in the sample
 */
int WAV_Channels(WAVInfo_t W) {
  return W->WAVHead.channels;
}

/*WAV_BitsPerSample*/
/* Return number of channels in the sample
 */
short WAV_BitsPerSample(WAVInfo_t W) {
  return W->WAVHead.wBitsPerSample;
}


/*WAV_SampleSpeed*/
/* Return sample frequency
 */
long WAV_SampleFreq(WAVInfo_t W) {
  return W->WAVHead.SamplesPerSec;
}


/*WAV_SetData*/
/* Take the data array and convert it
 *  back to the raw data format
 */
void WAV_SetData(WAVInfo_t W,void *Data, long Elements) {
  long t;
  
  switch(W->WAVHead.wBitsPerSample) {
  case 8:
  {
     unsigned char *CData=(unsigned char *)Data;

     W->WAVHead.datalen=Elements;

     if((W->RawData=realloc(W->RawData,W->WAVHead.datalen))==NULL)
       Error("realloc failed");

     for(t=0;t<Elements;t++) {
       ((unsigned char *)W->RawData)[t]=CData[t]+128;
     }
     break;
    
  }
  case 16:
  {
    short *SData=(short *)Data;

    W->WAVHead.datalen=Elements*sizeof(short);

    if((W->RawData=realloc(W->RawData,W->WAVHead.datalen))==NULL)
      Error("realloc failed");

    for(t=0;t<Elements;t++) {
      ((short *)W->RawData)[t]=SData[t];
      FixEndianS(&((short *)W->RawData)[t]);
    }
    break;
  }
  default:
    Error("Internal error: This sample file has an unsupported number of bits");
  }

}

/*WAV_Free*/
/* Free all memory associated with
 *  the wav file.
 */
void WAV_Free(WAVInfo_t W) {
  wavcuepoint_t *c,*d;

  c=W->CueList;

  while(c!=NULL) {
    d=c;
    c=c->Next;
    free(d);
  }
  
  free(W->RawData);
  free(W);
}


/*FixEndianS*/
/* Change the endian for a short
 */
static void FixEndianS(short *n) {
  short r=*n;
  
  #ifdef __sparc__
  ((char *)&r)[0]=((char *)n)[1];
  ((char *)&r)[1]=((char *)n)[0];
  #endif

 *n=r;
}


/*FixEndianL*/
/* Change the endian for a long
 */
static void FixEndianL(long *n) {
  long r=*n;
  
  #ifdef __sparc__
  ((char *)&r)[0]=((char *)n)[3];
  ((char *)&r)[1]=((char *)n)[2];
  ((char *)&r)[2]=((char *)n)[1];
  ((char *)&r)[3]=((char *)n)[0];
  #endif

 *n=r;
}

