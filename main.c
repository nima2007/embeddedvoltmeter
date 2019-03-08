//blaaaa

#include <avr/io.h>
#include "stdio.h"
#include "avr.h"
#include "keypad.h"
#include "music.h"
#include "lcd.h"

char bufMsg[17];

extern enum keys
{
	Key1, Key2, Key3, KeyA,
	Key4, Key5, Key6, KeyB,
	Key7, Key8, Key9, KeyC,
	KeyStar, Key0, KeyPound, KeyD
};

void blank_meter()
{
	sprintf(bufMsg, "ins:____avg:____");
	lcd_pos(0, 0);
	lcd_puts2(bufMsg);

	sprintf(bufMsg, "min:____max:____");
	lcd_pos(1, 0);
	lcd_puts2(bufMsg);

	while(keypad_get_key() != 5)
	{
		avr_wait(10);
	}
}

unsigned short get_sample()
{
	ADMUX |= (1 << REFS0);
	//ADMUX |= (1 << 3);
	//ADMUX |= (1 << 3);
	ADMUX |= (1 << 4);
	SET_BIT(ADCSRA, 7); //Set ADEN
	SET_BIT(ADCSRA, 6); //Set ADSC start conversion
	while (GET_BIT(ADCSRA, 6)); //wait to finish measure

	return ADC;


}


void welcome()
{
	lcd_clr();
	lcd_pos(0, 0);
	char buf[17];
	sprintf(buf, "ByeJose Meter");
	avr_wait(500);
	lcd_puts2(buf);
	lcd_pos(1, 0);
	sprintf(buf, "V1.00");
	lcd_puts2(buf);
	avr_wait(1000);

}

void playalarm()
{
	playsingle(A, .08);
	playsingle(G, .08);
	
}

int main(void)
{
	avr_init();
	lcd_init();
	welcome();
	blank_meter();
	float currentv = 0.0;
	float averagev = 0.0;
	float minv = 0.0;
	float maxv = 0.0;
	float total = 0.0;
	unsigned short counter = 0;

	while (1==1)
	{
		avr_wait(10);
		if (keypad_get_key() == 5)
		{
			while (1==1)
			{
				if (keypad_get_key() == 8)
				{
					blank_meter();
					maxv = 0.0;
					total = 0.0;
					counter = 0;
					currentv = 0.0;
					averagev = 0.0;
					minv = 0.0;
				}

				//curr = (float) get_sample()   / 1023.0 * 5.0 ;
				currentv = (float) get_sample() /496.0 * 5.0;
				//float x = curr;
				//curr = .000006 * x * x - 0.0016 * x - 0.024;
				
				if(currentv > 4.5 || currentv < .5)
				{
					playalarm();
					
				}


				counter++;

				if (counter == 1)
				{
					minv = currentv;
					maxv = currentv;
				}

				if (currentv < minv)
				{
					minv = currentv;
				}

				else if (currentv > maxv)
				{
					maxv = currentv;
					if(maxv>5)
						maxv = 5;
				}
				

				total += currentv;
				averagev = total / (float) counter;

				sprintf(bufMsg, "Ins:%.2fAvg:%.2f", currentv, averagev);
				lcd_pos(0, 0);
				lcd_puts2(bufMsg);

				sprintf(bufMsg, "Min:%.2fMax:%.2f", minv, maxv);
				lcd_pos(1, 0);
				lcd_puts2(bufMsg);

				avr_wait(500);
			}
		}
	}
}

