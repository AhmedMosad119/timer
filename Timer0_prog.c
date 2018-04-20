#include "Bit_Min.h"
#include "Datatypes.h"
#include <avr/io.h>
#include "DIO_Int.h"
#include <avr/interrupt.h>
#include <util/delay.h>
# define F_CPU 8000000UL
#include "LCD_prog.h"
#define Timer1_1stRising 0
#define Timer1_2stRising 1
#define Timer1_Tcount_ms 128
#define TOV Timer1_Tcount_ms*65500



void Timer0_Initialize_Normal();
void Timer1_Initialize_Normal();
u8 ov_counter =0;
int main (void)
{
	Timer0_Initialize_Normal();
	Lcd_vInitialization();
	DIO_vSETPinDirection(DIO_PORTB,DIO_PIN3,DIO_OUTPUT);

	while (1)
	{
	}
}

void Timer0_Initialize_Normal()
{

	Set_Bit(TCCR0,WGM01);
	Set_Bit(TCCR0,WGM00);

	//1024 prescaller
	Set_Bit(TCCR0,CS02);
	//Clr_Bit(TCCR0,CS01);
	Set_Bit(TCCR0,CS00);
	Set_Bit(TCCR0,COM01);
	Clr_Bit(TCCR0,COM00);
	TIMSK|=(1<<TOIE0);
	Set_Bit(SREG,7);
	OCR0=128;

}
void Timer1_Initialize_Normal()
{
	Set_Bit(TCCR1A,COM1A1);
	Set_Bit(TCCR1A,WGM11);
	Set_Bit(TCCR1B,WGM13);
	Set_Bit(TCCR1B,CS12); // prescalller
	ICR1=313; // top value
	OCR1A=16;
	DIO_vSETPinDirection(DIO_PORTD,DIO_PIN5,DIO_OUTPUT);
}

ISR(TIMER0_OVF_vect)
{
	ov_counter++;
}
ISR(TIMER1_CAPT_vect)
{
static u8 state =0;
static u16 t_total;
static u16 freq;
if (state == Timer1_1stRising)
{
	ov_counter=0;
	TCNT1=0;
	state=Timer1_2stRising;
}else
{
	t_total =Timer1_Tcount_ms*TCNT1+ ov_counter*TOV;
	freq = 1/t_total;
	state=Timer1_1stRising;
}

}




//ISR(ICR1)



/*
ISR(TIMER0_COMP_vect)
{
	u8 static counter =0;
	counter++;
	if (counter == 78)
	{
		Toggle_Bit(PORTD,7);
		counter=0;
	}
}
 */
