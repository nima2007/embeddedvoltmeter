//blaaaa

#include <avr/io.h>
#include "stdio.h"
#include "avr.h"
#include "keypad.h"
#include "lcd.h"

char bufMsg[17];

void ini_meter(){
	sprintf(bufMsg, "Ins:____Avg:____");
	lcd_pos(0,0);
	lcd_puts2(bufMsg);
	
	sprintf(bufMsg, "Min:____Max:____");
	lcd_pos(1,0);
	lcd_puts2(bufMsg);
}

unsigned short get_A2D(){
	ADMUX |= (1<<REFS0);
	SET_BIT(ADCSRA, 7); //Set ADEN
	SET_BIT(ADCSRA, 6); //Set ADSC start conversion
	while(GET_BIT(ADCSRA, 6)); //wait to finish measure
	
	return ADC;
	
	

}


void welcome()
{
	lcd_clr();
	lcd_pos(0,0);
	char buf[17];
	sprintf(buf, "ByeJose Meter");
	avr_wait(500);
	lcd_puts2(buf);
	lcd_pos(1, 0);
	sprintf(buf, "V1.00");
	lcd_puts2(buf);
	avr_wait(1000);
	
}

int main(void)
{
	avr_init();
	lcd_init();
	ini_meter();
	float curr = 0.0;
	float avg = 0.0;
	float min = 0.0;
	float max = 0.0;
	float total = 0.0;
	unsigned short count = 0;

	while (1)
	{
		if(keypad_get_key() == 4){
			while(1){
				if(keypad_get_key() == 8){
					ini_meter();
					curr = 0.0;
					avg = 0.0;
					min = 0.0;
					max = 0.0;
					total = 0.0;
					count = 0;
					break;
				}
				
				curr = (float)get_A2D() / 1023.0 * 5.0;
				++count;
				
				if(count == 1){
					min = curr;
					max = curr;
				}

				if (curr < min){
					min = curr;
				}
				
				if(curr > max){
					max = curr;
				}
				
				total += curr;
				avg = total / (float)count;
				
				sprintf(bufMsg, "Ins:%.2fAvg:%.2f", curr, avg);
				lcd_pos(0,0);
				lcd_puts2(bufMsg);
				
				sprintf(bufMsg, "Min:%.2fMax:%.2f", min, max);
				lcd_pos(1,0);
				lcd_puts2(bufMsg);
				
				avr_wait(500);
			}
		}
	}
}

