#ifndef __voiceBroadcast_H
#define __voiceBroadcast_H

#include "stm32f10x.h"

#define DEBUG2				0

void voiceBroadcastINIT(uint32_t BaudRate);
void voiceBroadcastEnterAndExitLowPowerMode(void);
void voiceBroadcastSetVolume(uint8_t Volume);
void voiceBroadcastSetEQMode(uint8_t EQ);
uint8_t voiceBroadcastStatus(void);
void voiceBroadcastSetRepetitionMode(uint8_t Mode);
void voiceBroadcastPointStartPosition(uint8_t file);
void playingMusic(void);
uint8_t GetpVStatus(void);
void SetpVStatus(_Bool status);


#endif
