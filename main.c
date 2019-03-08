

#include <avr/io.h>
#include "stdio.h"
#include "avr.h"
#include "keypad.h"
#include "lcd.h"

char bufMsg[17];

unsigned int is_pressed(int r, int c)
{
	//Set to default
	DDRC = 0;

	c += 4;
	//Set row to output and STRONG 0
	SET_BIT(DDRC, r);
	CLR_BIT(PORTC, r);

	//Set column to input and WEAK 1
	CLR_BIT(DDRC, c);
	SET_BIT(PORTC, c);

	return (GET_BIT(PINC, c)) == 0 ? 1 : 0;
}

unsigned int get_key()
{
	int r, c;
	for (r = 0; r < 4; ++r)
	{
		for (c = 0; c < 4; ++c)
		{
			if (is_pressed(r, c))
			return r * 4 + c + 1;
		}
	}
	return 0;
}


void ini_meter(){
	sprintf(bufMsg, "Ins:____Avg:____");
	lcd_pos(0,0);
	lcd_puts2(bufMsg);
	
	sprintf(bufMsg, "Min:____Max:____");
	lcd_pos(1,0);
	lcd_puts2(bufMsg);
}

unsigned short get_reading(){
	ADMUX |= (1<<REFS0);
	SET_BIT(ADCSRA, 7);
	SET_BIT(ADCSRA, 6); 
	while(GET_BIT(ADCSRA, 6)); 
	
	return ADC;
}

void welcome()
{
	lcd_clr();
	lcd_pos(0,0);
	char buf[17];
	sprintf(buf, "ByeJose Meter");
	wait_avr(500);
	lcd_puts2(buf);
	lcd_pos(1, 0);
	sprintf(buf, "V1.00");
	lcd_puts2(buf);
	wait_avr(1000);
	
}

int main(void)
{
	ini_avr();
	ini_lcd();
	ini_meter();
	welcome();
	float curr = 0.0;
	float avg = 0.0;
	float min = 0.0;
	float max = 0.0;
	float total = 0.0;
	unsigned short count = 0;

	while (1)
	{
		if(get_key() == 4){
			while(1){
				if(get_key() == 8){
					ini_meter();
					curr = 0.0;
					avg = 0.0;
					min = 0.0;
					max = 0.0;
					total = 0.0;
					count = 0;
					break;
				}
				
				curr = (float)get_reading() / 1023.0 * 5.0;
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
				avg = 1.1;
				
				sprintf(bufMsg, "Ins:%.2fAvg:%.2f", curr, avg);
				lcd_pos(0,0);
				lcd_puts2(bufMsg);
				
				sprintf(bufMsg, "Min:%.2fMax:%.2f", min, max);
				lcd_pos(1,0);
				lcd_puts2(bufMsg);
				
				wait_avr(500);
			}
		}
	}
}

