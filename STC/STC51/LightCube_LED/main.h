#ifndef __MAIN_H
#define __MAIN_H

#include "stc8h.h"
#include "intrins.h"

#define CKSEL       (*(unsigned char volatile xdata *)0xfe00)
#define CLKDIV      (*(unsigned char volatile xdata *)0xfe01)
	
//�±�ΪSTC8G1K08�Ĳ����б�
#define ID_ROMADDR      ((unsigned char code *)0x1ff9)
#define VREF_ROMADDR    (*(unsigned int code *)0x1ff7)
#define F32K_ROMADDR    (*(unsigned int code *)0x1ff5)
#define T22M_ROMADDR    (*(unsigned char code *)0x1ff4) //22.1184MHz
#define T24M_ROMADDR    (*(unsigned char code *)0x1ff3) //24MHz
#define T20M_ROMADDR    (*(unsigned char code *)0x1ff2) //20MHz
#define T27M_ROMADDR    (*(unsigned char code *)0x1ff1) //27MHz
#define T30M_ROMADDR    (*(unsigned char code *)0x1ff0) //30MHz
#define T33M_ROMADDR    (*(unsigned char code *)0x1fef) //33.1776MHz
#define T35M_ROMADDR    (*(unsigned char code *)0x1fee) //35MHz
#define T36M_ROMADDR    (*(unsigned char code *)0x1fed) //36.864MHz
#define VRT20M_ROMADDR  (*(unsigned char code *)0x1fea) //VRTRIM_20M
#define VRT35M_ROMADDR  (*(unsigned char code *)0x1fe9) //VRTRIM_35M
	
//sfr     T2L     =   0xd7;
//sfr     T2H     =   0xd6;
//sfr     AUXR    =   0x8e;
//sfr     IE2     =   0xaf;
#define ET2         0x04
//sfr     AUXINTIF =  0xef;
#define T2IF        0x01
//	
//sfr     P_SW2   =   0xba;
//sfr     IRCBAND =   0x9d;
//sfr     IRTRIM  =   0x9f;
sfr     VRTRIM  =   0xa6;

////sfr     AUXR    =   0x8e;

//sfr     P0M1    =   0x93;
//sfr     P0M0    =   0x94;
//sfr     P1M1    =   0x91;
//sfr     P1M0    =   0x92;
//sfr     P2M1    =   0x95;
//sfr     P2M0    =   0x96;
//sfr     P3M1    =   0xb1;
//sfr     P3M0    =   0xb2;
//sfr     P4M1    =   0xb3;
//sfr     P4M0    =   0xb4;
//sfr     P5M1    =   0xc9;
//sfr     P5M0    =   0xca;

#endif
