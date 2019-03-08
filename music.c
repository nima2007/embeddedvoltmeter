// Project 3 Jose&Nima

#include "music.h"


extern enum keys
{Key1, Key2, Key3, KeyA,
	Key4, Key5, Key6, KeyB,
	Key7, Key8, Key9, KeyC,
KeyStar, Key0, KeyPound, KeyD};

extern enum tf { False, True};


void clrTop(){
	sprintf(bufmsg,"");
	lcd_pos(0,0);
	lcd_puts2(bufmsg);
}

void clrBot(){
	sprintf(bufmsg,"");
	lcd_pos(1,0);
	lcd_puts2(bufmsg);
}

void play(songs song){
	lcd_clr();
	
	//display song name
	//sprintf(bufmsg,"");
	sprintf(bufmsg, "%s", "Now Playing");
	lcd_pos(0,0);
	lcd_puts2(bufmsg);
	
	//temp and volume
	//sprintf(bufmsg,"");
	sprintf(bufmsg, "Vol: %d ", vol);
	lcd_pos(1,0);
	lcd_puts2(bufmsg);
	
	//sprintf(bufmsg,"");
	sprintf(bufmsg, "Tempo: %d ", tempo);
	lcd_pos(1,8);
	lcd_puts2(bufmsg);
	
	unsigned int pressed = 0;
	char needsupdate = 0;
	for (volatile unsigned int i = 0; i < song.length; ++i){
		pressed = get_key();
		avr_wait_tiny(5);
		if(pressed){
			
			switch (pressed)
			{

				case Key6: if (vol < 4)
				{
					vol = vol +1;
					needsupdate = True;
				}
				break;
				case Key9: if (vol > 1)
				{
					vol = vol -1;
					needsupdate = True;
				}
				break;
				case Key5: if (tempo < 10)
				{
					tempo = tempo +1;
					needsupdate = True;
				}
				break;
				case Key8: if (tempo > 1)
				{
					
					tempo = tempo -1;
					needsupdate = True;
				}
				break;
				default:
				break;
			}
			
			if(needsupdate == True){
				//sprintf(bufmsg,"");
				if(tempo > 5){
					tempo = 5;
				}
				
				sprintf(bufmsg, "%d ", vol);
				lcd_pos(1,5);
				lcd_puts2(bufmsg);
				
				//sprintf(bufmsg,"");
				sprintf(bufmsg, "%d ", tempo);
				lcd_pos(1,15);
				lcd_puts2(bufmsg);
			}
		}
		
		playsingle(song.sequence[i].frequency, song.sequence[i].duration);
		avr_wait_tiny(100); //wait 1ms
	}
}

void playsingle(unsigned int freq, float duration){
	SET_BIT(DDRB, 4);
	//each wait is 50us
	//if we define a bar of music to be 1 second
	//a whole note takes1 second, so W = 20,000 waits
	//ok = duration * 8 * 20000 bc duration is multiple of 8
	//tempo is increasing from .1 to 2
	//int ok = duration * 8 * 20000 / ((float)(tempo*3)/10)/440
	//change volume
	short mult = 0;
	switch(tempo)
	{
		case 1: mult = 500; break;
		case 2: mult = 0; break;
		case 3: mult = -500; break;
		case 4: mult = -1000; break;
		case 5: mult = -1500; break;
		default: mult = 0; break;
	}
	
	unsigned int Thi = 1/(freq*2*0.00001);
	unsigned int Tlo = Thi;
	
	int ok = (duration * (100000/Thi)) + mult;
	
	//rest if needed
	if(freq == 0)
	{
		avr_wait(duration);
		return;
	}
	
	
	for (int i = 0; i < ok ; ++i){
		SET_BIT(PORTB, 4);
		avr_wait_tiny(Thi); //TH
		CLR_BIT(PORTB, 4);
		avr_wait_tiny(Tlo); //TL
	}
}