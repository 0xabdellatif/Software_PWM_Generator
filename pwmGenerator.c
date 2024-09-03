#include <p18f452.h>
#pragma config WDT = OFF
#define MAX_HIGH_LOW 0xB1E0			// GENERATING 20 MS PERIOD

unsigned char timer0 = 0x00, button = 0x00, change = 0x00;
unsigned int multiByteOld = 0x00, multiByteNew = 0x00, interm = 0x00;

#pragma interrupt ISR
void ISR(void)
{
	if(INTCONbits.INT0IF)
	{
		change = 0x01;
		INTCONbits.INT0IF = 0;
		if(!button)
		{
			button = 0x01;
			multiByteNew = 0xB9B0;		// 90 DUTY CYCLE
			multiByteOld = 0xC568;
		}
		else if(button == 0x01)
		{
			button = 0x02;
			multiByteNew = 0xC568;		//	75 DUTY CYCLE
			multiByteOld = 0xB9B0;
		}
		else if(button == 0x02)
		{
			button = 0x03;
			multiByteNew = 0xD8F0;		// 50 DUTY CYCLE
			multiByteOld = 0xC568;
		}
		else if(button == 0x03)
		{
			button = 0x04;
			multiByteNew = 0xEC78;		// 25 DUTY CYCLE
			multiByteOld = 0xD8F0;
		}
		else if(button == 0x04)
		{
			button = 0x00;
			multiByteNew = 0xF830;		// 5% DUTY CYCLE
			multiByteOld = 0xEC78;
		}			
	}
	else if(INTCONbits.TMR0IF)
	{
		INTCONbits.TMR0IF = 0;
		if(!timer0)
		{
			timer0 = 0x01;
			TMR0H = (multiByteNew & 0xFF00) >> 8;
			TMR0L = (multiByteNew & 0x00FF);
			LATDbits.LATD0 = 1;
			T0CONbits.TMR0ON = 1;
		}
		else
		{
			if(change)												// IF INT0 IS PRESSED THEN THE NEW DUTY CYCLE WILL BE UPDATED AFTER THE END OF THE CURRENT PERIOD
			{
				interm = multiByteNew;
				multiByteNew = multiByteOld;	
			}
			timer0 = 0x00;
			TMR0H = ((MAX_HIGH_LOW - multiByteNew) & 0xFF00) >> 8;	// SUBTRUCT THE HIGH PERIOD FROM THE WHOLE PERIOD FOR LOW PORTION
			TMR0L = ((MAX_HIGH_LOW - multiByteNew) & 0x00FF);
			LATDbits.LATD0 = 0;	
			if(change)
			{
				multiByteNew = interm;		
				change = 0x00;
			}
		}
	}
}

#pragma code VECTOR = 0x00008				// ISR WITH AT ADDRESS 0x08 (HIGH PRIORITY)
void VECTOR(void)
{
	_asm
		GOTO ISR
	_endasm
}
#pragma code

void main(void)
{
	TRISDbits.TRISD0 = 0;				// OUTPUT RD0
	INTCONbits.GIE = 1;					// GLOBAL INTERRUPT ENABLE BIT
	INTCONbits.INT0IE = 1;
	INTCONbits.INT0IF = 0;
	INTCONbits.TMR0IE = 1;
	T0CON = 0x08;
	INTCONbits.INT0IF = 1;				// TRIGGER AN EXTERNAL HARDWARE INTERRUPT BY A SOFTWARE
	INTCONbits.TMR0IF = 1;				// TRIGGER TIMER INTERRUPT BY A SOFTWARE
	while(1);
}
