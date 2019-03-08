#include "keypad.h"

unsigned int keypad_is_pressed(int r, int c)
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

unsigned int keypad_get_key()
{
	int r, c;
	for (r = 0; r < 4; ++r)
	{
		for (c = 0; c < 4; ++c)
		{
			if (keypad_is_pressed(r, c))
			return r * 4 + c + 1;
		}
	}
	return 0;
}
