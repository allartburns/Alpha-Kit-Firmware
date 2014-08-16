/*
    5-19-08
    Nathan Seidle
    nathan@sparkfun.com
    Copyright Spark Fun Electronics© 2008
    
    Amplified Microphone Testing
    Basic AtoD on a PIC 16F88
*/
#define Clock_8MHz
#define Baud_9600

#include "c:\Global\Code\C\16F88.h"  // device dependent interrupt definitions

#pragma origin 4

#include "c:\Global\Code\Pics\Code\Delay.c"   //Standard delays
#include "c:\Global\Code\Pics\Code\stdio.c"   //Software based Basic Serial IO

#define STATUS_LED PORTB.3

void test_mic(void);
uns16 read_analog(uns8 channel);
void boot_up(void);

void main(void)
{
    boot_up();
        
    uns16 aud_level;

    //0 = Output, 1 = Input
    PORTA = 0b.0000.0000;
    TRISA = 0b.0000.0010; //GND on RA0, AUD on RA1

    while(1)
    {
        STATUS_LED ^= 1;

        aud_level = read_analog(1);
        
        if(aud_level > 520)
            printf("Audio level: %d\n", aud_level);
    }    

    while(1);
}//End Main

void boot_up(void)
{
    //Setup Ports
    ANSEL = 0b.0000.0000; //Turn off A/D

    PORTA = 0b.0000.0000;
    TRISA = 0b.0000.0000;

    PORTB = 0b.0000.0000;
    TRISB = 0b.0000.0100;   //0 = Output, 1 = Input RX on RB2

    //Setup the hardware UART module
    //=============================================================
    SPBRG = 51; //8MHz for 9600 inital communication baud rate
    //SPBRG = 59; //9.216MHz for 9600 inital communication baud rate
    //SPBRG = 4; //9.216MHz for 115200 inital communication baud rate
    //SPBRG = 129; //20MHz for 9600 inital communication baud rate

    TXSTA = 0b.0010.0100; //8-bit asych mode, high speed uart enabled
    RCSTA = 0b.1001.0000; //Serial port enable, 8-bit asych continous receive mode
    //=============================================================
}

uns16 read_analog(uns8 channel)
{
    uns16 temp;
    uns8 chan_number;
    
    chan_number = (1<<channel); //Turn on A/D
    ANSEL = 0x00 | chan_number;
    
    channel <<= 3; //Shift channel to align with ADCON0

    ADCON0 = 0b.1000.0001 | channel; //ADCS 10 32/Osc, Select channel
    ADCON1 = 0x80; //Left justified
    
    delay_ms(1);
    
    GO = 1; //Convert to digital
    
    while(GO == 1);
    
    temp.high8 = ADRESH;
    temp.low8 = ADRESL;
    
    return(temp);
}
