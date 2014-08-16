
; CC5X Version 3.1I, Copyright (c) B Knudsen Data
; C compiler for the PICmicro family
; ************  19. May 2008  18:53  *************

	processor  16F88
	radix  DEC

INDF        EQU   0x00
PCL         EQU   0x02
FSR         EQU   0x04
PORTA       EQU   0x05
TRISA       EQU   0x85
PORTB       EQU   0x06
TRISB       EQU   0x86
PCLATH      EQU   0x0A
Carry       EQU   0
Zero_       EQU   2
RP0         EQU   5
RP1         EQU   6
IRP         EQU   7
GIE         EQU   7
RCSTA       EQU   0x18
TXREG       EQU   0x19
RCREG       EQU   0x1A
ADRESH      EQU   0x1E
ADCON0      EQU   0x1F
TXSTA       EQU   0x98
SPBRG       EQU   0x99
ANSEL       EQU   0x9B
ADRESL      EQU   0x9E
ADCON1      EQU   0x9F
PEIE        EQU   6
TXIF        EQU   4
RCIF        EQU   5
CREN        EQU   4
SPEN        EQU   7
GO          EQU   2
TXIE        EQU   4
RCIE        EQU   5
BRGH        EQU   2
SYNC        EQU   4
TXEN        EQU   5
x           EQU   0x7F
x_2         EQU   0x26
y           EQU   0x28
z           EQU   0x29
x_3         EQU   0x7F
y_2         EQU   0x7F
want_ints   EQU   0
want_ints_2 EQU   0
nate        EQU   0x32
counter     EQU   0x7F
x_4         EQU   0x32
nate_2      EQU   0x22
my_byte     EQU   0x23
i           EQU   0x25
k           EQU   0x26
m           EQU   0x27
temp        EQU   0x28
high_byte   EQU   0x29
low_byte    EQU   0x2A
C1cnt       EQU   0x32
C2tmp       EQU   0x33
C3cnt       EQU   0x32
C4tmp       EQU   0x33
C5cnt       EQU   0x32
C6tmp       EQU   0x33
C7rem       EQU   0x35
aud_level   EQU   0x20
channel     EQU   0x22
temp_2      EQU   0x23
chan_number EQU   0x25
C8cnt       EQU   0x26
ci          EQU   0x32

	GOTO main

  ; FILE C:\Global\Code\16F88\Mic-Testing\Mic-Testing-v10.c
			;/*
			;    5-19-08
			;    Nathan Seidle
			;    nathan@sparkfun.com
			;    Copyright Spark Fun Electronics© 2008
			;    
			;    Amplified Microphone Testing
			;    Basic AtoD on a PIC 16F88
			;*/
			;#define Clock_8MHz
			;#define Baud_9600
			;
			;#include "c:\Global\Code\C\16F88.h"  // device dependent interrupt definitions
			;
			;#pragma origin 4
	ORG 0x0004

  ; FILE c:\Global\Code\Pics\Code\Delay.c
			;/*
			;    7/23/02
			;    Nathan Seidle
			;    nathan.seidle@colorado.edu
			;    
			;    Delays for... Well, everything.
			;    
			;    11-11 Updated the delays - now they actually delay what they say they should.
			;    
			;    10-11-03 Updated delays. New CC5X compiler is muy optimized.
			;
			;*/
			;
			;//Really short delay
			;void delay_us(uns16 x)
			;{
_const1
	MOVWF ci
	MOVLW .17
	SUBWF ci,W
	BTFSC 0x03,Carry
	RETLW .0
	CLRF  PCLATH
	MOVF  ci,W
	ADDWF PCL,1
	RETLW .65
	RETLW .117
	RETLW .100
	RETLW .105
	RETLW .111
	RETLW .32
	RETLW .108
	RETLW .101
	RETLW .118
	RETLW .101
	RETLW .108
	RETLW .58
	RETLW .32
	RETLW .37
	RETLW .100
	RETLW .10
	RETLW .0
delay_us
			;
			;#ifdef Clock_4MHz
			;    //Calling with 10us returns 69us
			;    for ( ; x > 0 ; x--);
			;#endif
			;
			;#ifdef Clock_8MHz
			;    //Calling with 1us returns 11us
			;    //Calling with 10us returns 56us
			;    //for ( ; x > 0 ; x--);
			;    
			;    //Calling with 1us returns 7.5us
			;    //Calling with 10us returns 48
			;    //Calling with 1000us returns 4.5ms
			;    while(--x); 
m001	DECF  x,1
	INCF  x,W
	BTFSC 0x03,Zero_
	DECF  x+1,1
	MOVF  x,W
	IORWF x+1,W
	BTFSS 0x03,Zero_
	GOTO  m001
			;
			;    //while(x--); 
			;#endif
			;
			;#ifdef Clock_9216MHz
			;    //Copied from 8MHz - not tested
			;    while(--x); 
			;#endif
			;
			;#ifdef Clock_10MHz
			;    //delay_us(1) = 6.4us
			;    //delay_us(10) = 16.8us
			;    //delay_us(100) = 110.4us
			;    while(x > 4) x -= 5;
			;#endif
			;
			;#ifdef Clock_20MHz
			;    //Calling with 10us returns 13 us
			;    //Calling with 1us returns 1.8us
			;    while(--x) nop(); 
			;#endif
			;
			;}
	RETURN
			;
			;//General short delay
			;void delay_ms(uns16 x)
			;{
delay_ms
			;
			;#ifdef Clock_4MHz
			;    //Clocks out at 1002us per 1ms
			;    int y;
			;    for ( ; x > 0 ; x--)
			;        for ( y = 0 ; y < 108 ; y++);
			;#endif
			;
			;#ifdef Clock_8MHz
			;    //Clocks out at 1006us per 1ms
			;    uns8 y, z;
			;    for ( ; x > 0 ; x--)
m002	MOVF  x_2,W
	IORWF x_2+1,W
	BTFSC 0x03,Zero_
	GOTO  m007
			;        for ( y = 0 ; y < 4 ; y++)
	CLRF  y
m003	MOVLW .4
	SUBWF y,W
	BTFSC 0x03,Carry
	GOTO  m006
			;            for ( z = 0 ; z < 69 ; z++);
	CLRF  z
m004	MOVLW .69
	SUBWF z,W
	BTFSC 0x03,Carry
	GOTO  m005
	INCF  z,1
	GOTO  m004
m005	INCF  y,1
	GOTO  m003
m006	DECF  x_2,1
	INCF  x_2,W
	BTFSC 0x03,Zero_
	DECF  x_2+1,1
	GOTO  m002
			;#endif
			;
			;#ifdef Clock_9216MHz
			;    //Copied from 8MHz - not tested
			;    uns8 y, z;
			;    for ( ; x > 0 ; x--)
			;        for ( y = 0 ; y < 4 ; y++)
			;            for ( z = 0 ; z < 69 ; z++);
			;#endif
			;
			;#ifdef Clock_10MHz
			;    //delay_ms(1) = 1.006ms
			;    //delay_ms(10) = 10.02ms
			;    //delay_ms(100) = 100.16ms
			;
			;    uns8 y, z;
			;    for ( ; x > 0 ; x--)
			;        for ( y = 0 ; y < 4 ; y++)
			;            for ( z = 0 ; z < 87 ; z++);
			;#endif
			;
			;#ifdef Clock_20MHz
			;
			;    uns8 y, z;
			;    //Clocks out to 1.00ms per 1ms
			;    //9.99 ms per 10ms
			;    for ( ; x > 0 ; x--)
			;        for ( y = 0 ; y < 4 ; y++)
			;            for ( z = 0 ; z < 176 ; z++);
			;#endif
			;
			;}
m007	RETURN
			;
			;//Delays in 31.25kHz Low Power mode using the internal 31.25kHz oscillator
			;void delay_s_lp(uns16 x)
			;{
delay_s_lp
			;
			;    uns16 y;
			;    //Clocks out to 1.001s per 1s
			;    for ( ; x > 0 ; x--)
m008	MOVF  x_3,W
	IORWF x_3+1,W
	BTFSC 0x03,Zero_
	GOTO  m012
			;        for ( y = 0 ; y < 775 ; y++);
	CLRF  y_2
	CLRF  y_2+1
m009	MOVLW .3
	SUBWF y_2+1,W
	BTFSS 0x03,Carry
	GOTO  m010
	BTFSS 0x03,Zero_
	GOTO  m011
	MOVLW .7
	SUBWF y_2,W
	BTFSC 0x03,Carry
	GOTO  m011
m010	INCF  y_2,1
	BTFSC 0x03,Zero_
	INCF  y_2+1,1
	GOTO  m009
m011	DECF  x_3,1
	INCF  x_3,W
	BTFSC 0x03,Zero_
	DECF  x_3+1,1
	GOTO  m008

  ; FILE C:\Global\Code\16F88\Mic-Testing\Mic-Testing-v10.c
			;
			;#include "c:\Global\Code\Pics\Code\Delay.c"   //Standard delays
m012	RETURN

  ; FILE c:\Global\Code\Pics\Code\stdio.c
			;/*
			;    5/21/02
			;    Nathan Seidle
			;    nathan.seidle@colorado.edu
			;    
			;    Serial Out Started on 5-21
			;    rs_out Perfected on 5-24
			;    
			;    1Wire Serial Comm works with 4MHz Xtal
			;    Connect Serial_Out to Pin2 on DB9 Serial Connector
			;    Connect Pin5 on DB9 Connector to Signal Ground
			;    9600 Baud 8-N-1
			;    
			;    5-21 My first real C and Pic program.
			;    5-24 Attempting 20MHz implementation
			;    5-25 20MHz works
			;    5-25 Serial In works at 4MHz
			;    5-25 Passing Strings 9:20
			;    5-25 Option Selection 9:45
			;
			;    6-9  'Stdio.c' created. Printf working with %d and %h
			;    7-20 Added a longer delay after rs_out
			;         Trying to get 20MHz on the 16F873 - I think the XTal is bad.
			;         20MHz also needs 5V Vdd. Something I dont have.
			;    2-9-03 Overhauled the 4MHz timing. Serial out works very well now.
			;    
			;    6-16-03 Discovered how to pass string in cc5x
			;        void test(const char *str);
			;        test("zbcdefghij"); TXREG = str[1];
			;        
			;        Moved to hardware UART. Old STDIO will be in goodworks.
			;        
			;        Works great! Even got the special print characters (\n, \r, \0) to work.
			;    
			;    4-25-04 Added new %d routine to print 16 bit signed decimal numbers without leading 0s.
			;        
			;
			;*/
			;
			;//Setup the hardware UART TX module
			;void enable_uart_TX(bit want_ints)
			;{
enable_uart_TX
			;    BRGH = 0; //Normal speed UART
	BSF   0x03,RP0
	BCF   0x03,RP1
	BCF   0x98,BRGH
			;
			;    SYNC = 0;
	BCF   0x98,SYNC
			;    SPEN = 1;
	BCF   0x03,RP0
	BSF   0x18,SPEN
			;
			;#ifdef Clock_4MHz
			;    #ifdef Baud_9600
			;    SPBRG = 6; //4MHz for 9600 Baud
			;    #endif
			;#endif
			;
			;#ifdef Clock_8MHz
			;    #ifdef Baud_4800
			;    SPBRG = 25; //8MHz for 4800 Baud
			;    #endif
			;    #ifdef Baud_9600
			;    SPBRG = 12; //8MHz for 9600 Baud
	MOVLW .12
	BSF   0x03,RP0
	MOVWF SPBRG
			;    #endif
			;    #ifdef Baud_57600
			;    BRGH = 1; //High speed UART
			;    SPBRG = 7; //8MHz for 57600 Baud
			;    #endif
			;#endif
			;
			;#ifdef Crazy_Osc
			;    #ifdef Baud_9600
			;    SPBRG = 32; //20MHz for 9600 Baud
			;    #endif
			;#endif
			;
			;#ifdef Clock_20MHz
			;    #ifdef Baud_9600
			;    SPBRG = 31; //20MHz for 9600 Baud
			;    #endif
			;
			;    #ifdef Baud_4800
			;    SPBRG = 64; //20MHz for 4800 Baud
			;    #endif
			;#endif
			;
			;    if(want_ints) //Check if we want to turn on interrupts
	BTFSS 0x7F,want_ints
	GOTO  m013
			;    {
			;        TXIE = 1;
	BSF   0x8C,TXIE
			;        PEIE = 1;
	BSF   0x0B,PEIE
			;        GIE = 1;
	BSF   0x0B,GIE
			;    }
			;
			;    TXEN = 1; //Enable transmission
m013	BSF   0x98,TXEN
			;}    
	RETURN
			;
			;//Setup the hardware UART RX module
			;void enable_uart_RX(bit want_ints)
			;{
enable_uart_RX
			;
			;    BRGH = 0; //Normal speed UART
	BSF   0x03,RP0
	BCF   0x03,RP1
	BCF   0x98,BRGH
			;
			;    SYNC = 0;
	BCF   0x98,SYNC
			;    SPEN = 1;
	BCF   0x03,RP0
	BSF   0x18,SPEN
			;
			;#ifdef Clock_4MHz
			;    #ifdef Baud_9600
			;    SPBRG = 6; //4MHz for 9600 Baud
			;    #endif
			;#endif
			;
			;#ifdef Clock_8MHz
			;    #ifdef Baud_4800
			;    SPBRG = 25; //8MHz for 4800 Baud
			;    #endif
			;    #ifdef Baud_9600
			;    SPBRG = 12; //8MHz for 9600 Baud
	MOVLW .12
	BSF   0x03,RP0
	MOVWF SPBRG
			;    #endif
			;    #ifdef Baud_57600
			;    BRGH = 1; //High speed UART
			;    SPBRG = 8; //8MHz for 57600 Baud
			;    #endif
			;#endif
			;
			;#ifdef Crazy_Osc
			;    #ifdef Baud_9600
			;    SPBRG = 32; //20MHz for 9600 Baud
			;    #endif
			;#endif
			;
			;#ifdef Clock_20MHz
			;    #ifdef Baud_9600
			;    SPBRG = 31; //20MHz for 9600 Baud
			;    #endif
			;
			;    #ifdef Baud_4800
			;    SPBRG = 64; //20MHz for 4800 Baud
			;    #endif
			;#endif
			;
			;    CREN = 1;
	BCF   0x03,RP0
	BSF   0x18,CREN
			;
			;    //WREN = 1;
			;
			;    if(want_ints) //Check if we want to turn on interrupts
	BTFSS 0x7F,want_ints_2
	GOTO  m014
			;    {
			;        RCIE = 1;
	BSF   0x03,RP0
	BSF   0x8C,RCIE
			;        PEIE = 1;
	BSF   0x0B,PEIE
			;        GIE = 1;
	BSF   0x0B,GIE
			;    }
			;
			;}    
m014	BSF   0x03,RP0
	RETURN
			;
			;//Sends nate to the Transmit Register
			;void putc(uns8 nate)
			;{
putc
	MOVWF nate
			;    while(TXIF == 0);
m015	BTFSS 0x0C,TXIF
	GOTO  m015
			;    TXREG = nate;
	MOVF  nate,W
	MOVWF TXREG
			;}
	RETURN
			;
			;uns8 getc(void)
			;{
getc
			;    while(RCIF == 0);
	BCF   0x03,RP0
	BCF   0x03,RP1
m016	BTFSS 0x0C,RCIF
	GOTO  m016
			;    return (RCREG);
	MOVF  RCREG,W
	RETURN
			;}    
			;
			;uns8 scanc(void)
			;{
scanc
			;    uns16 counter = 0;
	CLRF  counter
	CLRF  counter+1
			;    
			;    //CREN = 0;
			;    //CREN = 1;
			;    
			;    RCIF = 0;
	BCF   0x03,RP0
	BCF   0x03,RP1
	BCF   0x0C,RCIF
			;    while(RCIF == 0)
m017	BTFSC 0x0C,RCIF
	GOTO  m018
			;    {
			;        counter++;
	INCF  counter,1
	BTFSC 0x03,Zero_
	INCF  counter+1,1
			;        if(counter == 1000) return 0;
	MOVF  counter,W
	XORLW .232
	BTFSS 0x03,Zero_
	GOTO  m017
	MOVF  counter+1,W
	XORLW .3
	BTFSS 0x03,Zero_
	GOTO  m017
	RETLW .0
			;    }
			;    
			;    return (RCREG);
m018	MOVF  RCREG,W
	RETURN
			;}    
			;
			;//Returns ASCII Decimal and Hex values
			;uns8 bin2Hex(char x)
			;{
bin2Hex
	MOVWF x_4
			;   skip(x);
	CLRF  PCLATH
	MOVF  x_4,W
	ADDWF PCL,1
			;   #pragma return[16] = "0123456789ABCDEF"
	RETLW .48
	RETLW .49
	RETLW .50
	RETLW .51
	RETLW .52
	RETLW .53
	RETLW .54
	RETLW .55
	RETLW .56
	RETLW .57
	RETLW .65
	RETLW .66
	RETLW .67
	RETLW .68
	RETLW .69
	RETLW .70
			;}
			;
			;//Prints a string including variables
			;void printf(const char *nate, int16 my_byte)
			;{
printf
			;  
			;    uns8 i, k, m, temp;
			;    uns8 high_byte = 0, low_byte = 0;
	CLRF  high_byte
	CLRF  low_byte
			;    uns8 y, z;
			;    
			;    uns8 decimal_output[5];
			;    
			;    for(i = 0 ; ; i++)
	CLRF  i
			;    {
			;        //delay_ms(3);
			;        
			;        k = nate[i];
m019	MOVF  i,W
	ADDWF nate_2,W
	CALL  _const1
	MOVWF k
			;
			;        if (k == '\0') 
	MOVF  k,1
	BTFSC 0x03,Zero_
			;            break;
	GOTO  m045
			;
			;        else if (k == '%') //Print var
	XORLW .37
	BTFSS 0x03,Zero_
	GOTO  m043
			;        {
			;            i++;
	INCF  i,1
			;            k = nate[i];
	MOVF  i,W
	ADDWF nate_2,W
	CALL  _const1
	MOVWF k
			;
			;            if (k == '\0') 
	MOVF  k,1
	BTFSC 0x03,Zero_
			;                break;
	GOTO  m045
			;            else if (k == '\\') //Print special characters
	XORLW .92
	BTFSS 0x03,Zero_
	GOTO  m020
			;            {
			;                i++;
	INCF  i,1
			;                k = nate[i];
	MOVF  i,W
	ADDWF nate_2,W
	CALL  _const1
	MOVWF k
			;                
			;                putc(k);
	CALL  putc
			;                
			;
			;            } //End Special Characters
			;            else if (k == 'b') //Print Binary
	GOTO  m044
m020	MOVF  k,W
	XORLW .98
	BTFSS 0x03,Zero_
	GOTO  m025
			;            {
			;                for( m = 0 ; m < 8 ; m++ )
	CLRF  m
m021	MOVLW .8
	SUBWF m,W
	BTFSC 0x03,Carry
	GOTO  m044
			;                {
			;                    if (my_byte.7 == 1) putc('1');
	BTFSS my_byte,7
	GOTO  m022
	MOVLW .49
	CALL  putc
			;                    if (my_byte.7 == 0) putc('0');
m022	BTFSC my_byte,7
	GOTO  m023
	MOVLW .48
	CALL  putc
			;                    if (m == 3) putc(' ');
m023	MOVF  m,W
	XORLW .3
	BTFSS 0x03,Zero_
	GOTO  m024
	MOVLW .32
	CALL  putc
			;                    
			;                    my_byte = my_byte << 1;
m024	BCF   0x03,Carry
	RLF   my_byte,1
	RLF   my_byte+1,1
			;                }
	INCF  m,1
	GOTO  m021
			;            } //End Binary               
			;            else if (k == 'd') //Print Decimal
m025	MOVF  k,W
	XORLW .100
	BTFSS 0x03,Zero_
	GOTO  m039
			;            {
			;                //Print negative sign and take 2's compliment
			;                
			;                if(my_byte < 0)
	BTFSS my_byte+1,7
	GOTO  m028
			;                {
			;                    putc('-');
	MOVLW .45
	CALL  putc
			;                    my_byte *= -1;
	MOVF  my_byte,W
	MOVWF C2tmp
	MOVF  my_byte+1,W
	MOVWF C2tmp+1
	MOVLW .16
	MOVWF C1cnt
m026	BCF   0x03,Carry
	RLF   my_byte,1
	RLF   my_byte+1,1
	RLF   C2tmp,1
	RLF   C2tmp+1,1
	BTFSS 0x03,Carry
	GOTO  m027
	DECF  my_byte+1,1
	DECF  my_byte,1
	INCFSZ my_byte,W
	INCF  my_byte+1,1
m027	DECFSZ C1cnt,1
	GOTO  m026
			;                }
			;                
			;                
			;                if (my_byte == 0)
m028	MOVF  my_byte,W
	IORWF my_byte+1,W
	BTFSS 0x03,Zero_
	GOTO  m029
			;                    putc('0');
	MOVLW .48
	CALL  putc
			;                else
	GOTO  m044
			;                {
			;                    //Divide number by a series of 10s
			;                    for(m = 4 ; my_byte > 0 ; m--)
m029	MOVLW .4
	MOVWF m
m030	BTFSC my_byte+1,7
	GOTO  m037
	MOVF  my_byte,W
	IORWF my_byte+1,W
	BTFSC 0x03,Zero_
	GOTO  m037
			;                    {
			;                        temp = my_byte % (uns16)10;
	MOVF  my_byte,W
	MOVWF C4tmp
	MOVF  my_byte+1,W
	MOVWF C4tmp+1
	CLRF  temp
	MOVLW .16
	MOVWF C3cnt
m031	RLF   C4tmp,1
	RLF   C4tmp+1,1
	RLF   temp,1
	BTFSC 0x03,Carry
	GOTO  m032
	MOVLW .10
	SUBWF temp,W
	BTFSS 0x03,Carry
	GOTO  m033
m032	MOVLW .10
	SUBWF temp,1
m033	DECFSZ C3cnt,1
	GOTO  m031
			;                        decimal_output[m] = temp;
	MOVLW .45
	ADDWF m,W
	MOVWF FSR
	BCF   0x03,IRP
	MOVF  temp,W
	MOVWF INDF
			;                        my_byte = my_byte / (uns16)10;               
	MOVF  my_byte,W
	MOVWF C6tmp
	MOVF  my_byte+1,W
	MOVWF C6tmp+1
	CLRF  C7rem
	MOVLW .16
	MOVWF C5cnt
m034	RLF   C6tmp,1
	RLF   C6tmp+1,1
	RLF   C7rem,1
	BTFSC 0x03,Carry
	GOTO  m035
	MOVLW .10
	SUBWF C7rem,W
	BTFSS 0x03,Carry
	GOTO  m036
m035	MOVLW .10
	SUBWF C7rem,1
	BSF   0x03,Carry
m036	RLF   my_byte,1
	RLF   my_byte+1,1
	DECFSZ C5cnt,1
	GOTO  m034
			;                    }
	DECF  m,1
	GOTO  m030
			;                
			;                    for(m++ ; m < 5 ; m++)
m037	INCF  m,1
m038	MOVLW .5
	SUBWF m,W
	BTFSC 0x03,Carry
	GOTO  m044
			;                        putc(bin2Hex(decimal_output[m]));
	MOVLW .45
	ADDWF m,W
	MOVWF FSR
	BCF   0x03,IRP
	MOVF  INDF,W
	CALL  bin2Hex
	CALL  putc
	INCF  m,1
	GOTO  m038
			;                }
			;    
			;            } //End Decimal
			;            else if (k == 'h') //Print Hex
m039	MOVF  k,W
	XORLW .104
	BTFSS 0x03,Zero_
	GOTO  m041
			;            {
			;                //New trick 3-15-04
			;                putc('0');
	MOVLW .48
	CALL  putc
			;                putc('x');
	MOVLW .120
	CALL  putc
			;                
			;                if(my_byte > 0x00FF)
	BTFSC my_byte+1,7
	GOTO  m040
	MOVF  my_byte+1,1
	BTFSC 0x03,Zero_
	GOTO  m040
			;                {
			;                    putc(bin2Hex(my_byte.high8 >> 4));
	SWAPF my_byte+1,W
	ANDLW .15
	CALL  bin2Hex
	CALL  putc
			;                    putc(bin2Hex(my_byte.high8 & 0b.0000.1111));
	MOVLW .15
	ANDWF my_byte+1,W
	CALL  bin2Hex
	CALL  putc
			;                }
			;
			;                putc(bin2Hex(my_byte.low8 >> 4));
m040	SWAPF my_byte,W
	ANDLW .15
	CALL  bin2Hex
	CALL  putc
			;                putc(bin2Hex(my_byte.low8 & 0b.0000.1111));
	MOVLW .15
	ANDWF my_byte,W
	CALL  bin2Hex
	CALL  putc
			;
			;                /*high_byte.3 = my_byte.7;
			;                high_byte.2 = my_byte.6;
			;                high_byte.1 = my_byte.5;
			;                high_byte.0 = my_byte.4;
			;            
			;                low_byte.3 = my_byte.3;
			;                low_byte.2 = my_byte.2;
			;                low_byte.1 = my_byte.1;
			;                low_byte.0 = my_byte.0;
			;        
			;                putc('0');
			;                putc('x');
			;            
			;                putc(bin2Hex(high_byte));
			;                putc(bin2Hex(low_byte));*/
			;            } //End Hex
			;            else if (k == 'f') //Print Float
	GOTO  m044
m041	MOVF  k,W
	XORLW .102
	BTFSS 0x03,Zero_
	GOTO  m042
			;            {
			;                putc('!');
	MOVLW .33
	CALL  putc
			;            } //End Float
			;            else if (k == 'u') //Print Direct Character
	GOTO  m044
m042	MOVF  k,W
	XORLW .117
	BTFSS 0x03,Zero_
	GOTO  m044
			;            {
			;                //All ascii characters below 20 are special and screwy characters
			;                //if(my_byte > 20) 
			;                    putc(my_byte);
	MOVF  my_byte,W
	CALL  putc
			;            } //End Direct
			;                        
			;        } //End Special Chars           
			;
			;        else
	GOTO  m044
			;            putc(k);
m043	MOVF  k,W
	CALL  putc
			;    }    
m044	INCF  i,1
	GOTO  m019
			;}
m045	RETURN

  ; FILE C:\Global\Code\16F88\Mic-Testing\Mic-Testing-v10.c
			;#include "c:\Global\Code\Pics\Code\stdio.c"   //Software based Basic Serial IO
			;
			;#define STATUS_LED PORTB.3
			;
			;void test_mic(void);
			;uns16 read_analog(uns8 channel);
			;void boot_up(void);
			;
			;void main(void)
			;{
main
			;    boot_up();
	BSF   0x03,RP0
	BCF   0x03,RP1
	CALL  boot_up
			;        
			;    uns16 aud_level;
			;
			;    //0 = Output, 1 = Input
			;    PORTA = 0b.0000.0000;
	CLRF  PORTA
			;    TRISA = 0b.0000.0010; //GND on RA0, AUD on RA1
	MOVLW .2
	BSF   0x03,RP0
	MOVWF TRISA
			;
			;    while(1)
			;    {
			;        STATUS_LED ^= 1;
	BCF   0x03,RP0
m046	MOVLW .8
	XORWF PORTB,1
			;
			;        aud_level = read_analog(1);
	MOVLW .1
	CALL  read_analog
	MOVWF aud_level
	MOVF  temp_2+1,W
	MOVWF aud_level+1
			;        
			;        if(aud_level > 520)
	MOVLW .2
	SUBWF aud_level+1,W
	BTFSS 0x03,Carry
	GOTO  m046
	BTFSS 0x03,Zero_
	GOTO  m047
	MOVLW .9
	SUBWF aud_level,W
	BTFSS 0x03,Carry
	GOTO  m046
			;            printf("Audio level: %d\n", aud_level);
m047	CLRF  nate_2
	MOVF  aud_level,W
	MOVWF my_byte
	MOVF  aud_level+1,W
	MOVWF my_byte+1
	CALL  printf
			;    }    
	GOTO  m046
			;
			;    while(1);
m048	GOTO  m048
			;}//End Main
			;
			;void boot_up(void)
			;{
boot_up
			;    //Setup Ports
			;    ANSEL = 0b.0000.0000; //Turn off A/D
	CLRF  ANSEL
			;
			;    PORTA = 0b.0000.0000;
	BCF   0x03,RP0
	CLRF  PORTA
			;    TRISA = 0b.0000.0000;
	BSF   0x03,RP0
	CLRF  TRISA
			;
			;    PORTB = 0b.0000.0000;
	BCF   0x03,RP0
	CLRF  PORTB
			;    TRISB = 0b.0000.0100;   //0 = Output, 1 = Input RX on RB2
	MOVLW .4
	BSF   0x03,RP0
	MOVWF TRISB
			;
			;    //Setup the hardware UART module
			;    //=============================================================
			;    SPBRG = 51; //8MHz for 9600 inital communication baud rate
	MOVLW .51
	MOVWF SPBRG
			;    //SPBRG = 59; //9.216MHz for 9600 inital communication baud rate
			;    //SPBRG = 4; //9.216MHz for 115200 inital communication baud rate
			;    //SPBRG = 129; //20MHz for 9600 inital communication baud rate
			;
			;    TXSTA = 0b.0010.0100; //8-bit asych mode, high speed uart enabled
	MOVLW .36
	MOVWF TXSTA
			;    RCSTA = 0b.1001.0000; //Serial port enable, 8-bit asych continous receive mode
	MOVLW .144
	BCF   0x03,RP0
	MOVWF RCSTA
			;    //=============================================================
			;}
	RETURN
			;
			;uns16 read_analog(uns8 channel)
			;{
read_analog
	MOVWF channel
			;    uns16 temp;
			;    uns8 chan_number;
			;    
			;    chan_number = (1<<channel); //Turn on A/D
	MOVLW .1
	MOVWF chan_number
	MOVF  channel,W
	BTFSC 0x03,Zero_
	GOTO  m050
	MOVWF C8cnt
m049	BCF   0x03,Carry
	RLF   chan_number,1
	DECFSZ C8cnt,1
	GOTO  m049
			;    ANSEL = 0x00 | chan_number;
m050	MOVF  chan_number,W
	BSF   0x03,RP0
	MOVWF ANSEL
			;    
			;    channel <<= 3; //Shift channel to align with ADCON0
	BCF   0x03,Carry
	BCF   0x03,RP0
	RLF   channel,1
	BCF   0x03,Carry
	RLF   channel,1
	BCF   0x03,Carry
	RLF   channel,1
			;
			;    ADCON0 = 0b.1000.0001 | channel; //ADCS 10 32/Osc, Select channel
	MOVLW .129
	IORWF channel,W
	MOVWF ADCON0
			;    ADCON1 = 0x80; //Left justified
	MOVLW .128
	BSF   0x03,RP0
	MOVWF ADCON1
			;    
			;    delay_ms(1);
	MOVLW .1
	BCF   0x03,RP0
	MOVWF x_2
	CLRF  x_2+1
	CALL  delay_ms
			;    
			;    GO = 1; //Convert to digital
	BSF   0x1F,GO
			;    
			;    while(GO == 1);
m051	BTFSC 0x1F,GO
	GOTO  m051
			;    
			;    temp.high8 = ADRESH;
	MOVF  ADRESH,W
	MOVWF temp_2+1
			;    temp.low8 = ADRESL;
	BSF   0x03,RP0
	MOVF  ADRESL,W
	BCF   0x03,RP0
	MOVWF temp_2
			;    
			;    return(temp);
	RETURN

	END


; *** KEY INFO ***

; 0x001D P0    9 word(s)  0 % : delay_us
; 0x0026 P0   24 word(s)  1 % : delay_ms
; 0x003E P0   26 word(s)  1 % : delay_s_lp
; 0x0058 P0   16 word(s)  0 % : enable_uart_TX
; 0x0068 P0   19 word(s)  0 % : enable_uart_RX
; 0x007B P0    6 word(s)  0 % : putc
; 0x0081 P0    6 word(s)  0 % : getc
; 0x0087 P0   21 word(s)  1 % : scanc
; 0x009C P0   20 word(s)  0 % : bin2Hex
; 0x00B0 P0  215 word(s) 10 % : printf
; 0x0004 P0   25 word(s)  1 % : _const1
; 0x01BA P0   42 word(s)  2 % : read_analog
; 0x01A8 P0   18 word(s)  0 % : boot_up
; 0x0187 P0   33 word(s)  1 % : main

; RAM usage: 23 bytes (23 local), 345 bytes free
; Maximum call level: 2
;  Codepage 0 has  481 word(s) :  23 %
;  Codepage 1 has    0 word(s) :   0 %
; Total of 481 code words (11 %)
