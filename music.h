// Project 3 Jose&Nima

#ifndef __music__
#define __music__

#include "avr.h"
#include "stdio.h"
#include "lcd.h"

//     note delay(freq)
#define A 440
#define As 466
#define B 494
#define Bs 523
#define C 554
#define Cs 587
#define D 622
#define Ds 659
#define E 698
#define Es 740
#define F 784
#define Fs 831
#define G 880
#define Gs 932
#define P 1 //Pause

//      duration
#define W 1
#define H 0.5
#define Q 0.25
#define EI 0.125


char bufmsg[17];

unsigned int tempo;
unsigned int vol;

typedef struct{
	unsigned int frequency;
	float duration;
} note;

typedef struct {
	char title[17];
	note *sequence;
	unsigned int length;
}songs;


void play(songs song);
void clrLine(unsigned int r);
void playsingle(unsigned int freq, float duration);


#endif