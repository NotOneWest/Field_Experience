#include <gui/music_screen/MusicView.hpp>
#include <string.h>
#include "fatfs.h"

#include <iostream>
using namespace std;

#define AUDIO_BUFFER_SIZE 8820*2

typedef struct{
  uint32_t   ChunkID;       /* 0 */
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16*/
  uint16_t   AudioFormat;   /* 20 */
  uint16_t   NbrChannels;   /* 22 */
  uint32_t   SampleRate;    /* 24 */

  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */
  uint16_t   BitPerSample;  /* 34 */
  uint32_t   SubChunk2ID;   /* 36 */
  uint32_t   SubChunk2Size; /* 40 */
} WAVE_FormatTypeDef;

extern WAVE_FormatTypeDef waveformat;
extern char files[30][256];
extern int file_num, file_index;
extern int backState, pos, volume;
extern int playTime, audioPlay, play, goNext;

MusicView::MusicView()
{

}

void MusicView::setupScreen()
{
    MusicViewBase::setupScreen();
}

void MusicView::tearDownScreen()
{
    MusicViewBase::tearDownScreen();
}

void MusicView::SetMusicItem(int16_t itemSelected)
{
	playTime=0;
	title.invalidate();

	if(itemSelected >=0 && itemSelected < file_num){
		Unicode::UnicodeChar buffer[TITLE_SIZE];
		Unicode::strncpy(buffer, files[itemSelected], TITLE_SIZE);
		Unicode::snprintf(titleBuffer, TITLE_SIZE, "%s", buffer);

		file_index = itemSelected;
		res = f_mount(&SDFatFS, "", 0);
		res = f_open(&SDFile, files[file_index], FA_READ);
		if(res == FR_OK){
			f_read (&SDFile, &waveformat, sizeof(waveformat), &bytesread);
			playTime = (waveformat.FileSize / ((waveformat.SampleRate*waveformat.NbrChannels*waveformat.BitPerSample)/8));
		}

		Unicode::snprintf(playtimeBuffer1, PLAYTIMEBUFFER1_SIZE, "%d", playTime/60);
		Unicode::snprintf(playtimeBuffer2, PLAYTIMEBUFFER2_SIZE, "%02d", playTime%60);

		playBar.setRange(0, playTime*62);
	}
	title.resizeToCurrentTextWithAlignment(); title.invalidate();
	playtime.resizeToCurrentTextWithAlignment(); playtime.invalidate();
}

void MusicView::btnNextClicked()
{
	btnStopPlay.forceState(false); btnStopPlay.invalidate();

	playBar.setValue(0); audioPlay=0; pos=0;
	if(file_index < (file_num-1)) SetMusicItem(file_index+1);
	else SetMusicItem(0);

	btnStopPlay.forceState(true); btnStopPlay.invalidate();
}

void MusicView::btnPrevClicked()
{
	btnStopPlay.forceState(false); btnStopPlay.invalidate();

	playBar.setValue(0); pos=0; backState++;
	if(backState==2){
		audioPlay=0; backState=0;
		if(file_index > 0) SetMusicItem(file_index-1);
		else SetMusicItem(file_num-1);
	}

	btnStopPlay.forceState(true); btnStopPlay.invalidate();
}

void MusicView::playMusic()
{
	playBar.setValue(0);
	play = btnStopPlay.getState();
}

void MusicView::handleTickEvent()
{
	if(goNext){
		btnStopPlay.forceState(false); btnStopPlay.invalidate();
		if(file_index < (file_num-1)){ SetMusicItem(file_index+1); }
		playBar.setValue(0); goNext=0;
		btnStopPlay.forceState(true); btnStopPlay.invalidate();
	}

    int currentValue = playBar.getValue();
    int max, min;
    playBar.getRange(min, max);

    if(play && audioPlay==2){
    	if (currentValue == min) increase = true;
		else if (currentValue == max) increase = false;
    } else increase = false;

    int nextValue = (increase == true ? currentValue+1 : currentValue);
    playBar.setValue(nextValue);
}
