/*
SUNROOF MODULE
AUTHOR: TRAPTI DAMODAR BALGI
EMPLOYEE ID: 142907
VERSION: 1.0
DESCRIPTION: 1. The sunroof opens up when the open button is pressed
             2. The sunroof closes when the close button is pressed
             3. The sunroof opens up partially when the ventilation button is pressed
             4. When it rains, the sunroof closes automatically
*/
#include <avr/io.h>
#include<avr/interrupt.h>
#include<stdint.h>
#include<util/delay.h>
#define SET_BIT(PORT,PIN) PORT|=(1<<PIN)
#define CLR_BIT(PORT,PIN) PORT&=~(1<<PIN)
struct
{
    volatile unsigned int RAINSENSOR:1; //RAIN SENSOR INTERRUPT
}FLAG_BIT;

int main()
{
    SET_BIT(DDRC,PC0);      //MOTOR
    CLR_BIT(PORTC,PC0);

    CLR_BIT(DDRD,PD2);      //RAIN SENSOR
    CLR_BIT(PORTD,PD2);

    CLR_BIT(DDRD,PD0);      //CLOSE MODE
    CLR_BIT(PORTD,PD0);

    CLR_BIT(DDRD,PD1);      //OPEN MODE
    CLR_BIT(PORTD,PD1);

    CLR_BIT(DDRD,PD4);      //VENTILATION MODE
    CLR_BIT(PORTD,PD4);

    EIMSK|=(1<<INT0);       //CONFIGURING INTERRUPTS
    EICRA=(1<<ISC01 | 1<<ISC00);
    sei();

    while(1)
    {
        if(FLAG_BIT.RAINSENSOR)     //WHEN RAIN IS SENSED
        {
            PORTC = 0x01;
            _delay_us(1000);
            PORTC = 0x00;

            _delay_ms(2000);
        }
        else
        {
            uint8_t PIN_READ=0x00;
            PIN_READ=PIND;
            if(PIN_READ & (1<<PD0))         //WHEN CLOSE MODE IS SELECTED
            {
                PORTC = 0x01;
                _delay_us(1000);
                PORTC = 0x00;

                _delay_ms(2000);
            }
            else
                if(PIN_READ & (1<<PD1))     //WHEN OPEN MODE IS SELECTED
            {
                PORTC = 0x01;
                _delay_us(2000);
                PORTC = 0x00;
                _delay_ms(2000);
            }
            else
                if(PIN_READ & (1<<PD4))     //WHEN VENTILATION MODE IS SELECTED
            {
                PORTC = 0x01;
                _delay_us(2500);
                PORTC = 0x00;
                _delay_ms(2000);
            }
        }
    }
}
ISR(INT0_vect)
{
    FLAG_BIT.RAINSENSOR=1;
}


