/*
 * File:   main.c
 * Author: dlourenco
 *
 * Created on 28 de Março de 2021, 01:14
 */

#include <stdio.h>
#include <stdlib.h>
#include "main.h"
/*
void SPI_Init_Master_A()
{
    // Definição dos pinos SPI   
    TRISBbits.TRISB0 = 1;	// RB0 input(SDI) - RD14
    TRISBbits.TRISB1 = 0;	// RB1 output(SCK) - RF4
    TRISAbits.TRISA5 = 0;	// RA5 output(SS') - RF5
    TRISCbits.TRISC7 = 0;	// RC7 output(SDO) - RD15

    CS = 1;
    SSPSTAT = 0x40;		
    SSPCON1 = 0x22;		// Master mode, serial habilitada
    
    PIR1bits.SSPIF=0;
 
    ADCON0 = 0;			
    
    ADCON1 = 0x0F;		// Todos os pinos para I/O digital 
}

void SPI_Init_Master_B()
{
    // Definição dos pinos SPI   
    TRISBbits.TRISB0 = 1;	// RB0 input(SDI) - RD8
    TRISBbits.TRISB1 = 0;	// RB1 output(SCK) - RG13
    TRISAbits.TRISA5 = 0;	// RA5 output(SS') - RF5
    TRISCbits.TRISC7 = 0;	// RC7 output(SDO) - RD9

    CS = 1;
    SSPSTAT = 0x40;		
    SSPCON1 = 0x22;		// Master mode, serial habilitada
    
    PIR1bits.SSPIF=0;
 
    ADCON0 = 0;			
    
    ADCON1 = 0x0F;		// Todos os pinos para I/O digital 
}

void SPI_Write(unsigned char x)
{
    unsigned char data_flush;
    SSPBUF = x;			// Copia o dado em SSPBUF para transmitir

    while(!PIR1bits.SSPIF);	// Aguarda a transmissão de 1 byte
    PIR1bits.SSPIF=0;		// Limpa SSPIF
    data_flush = SSPBUF;
}

unsigned char SPI_Read()
{    
    SSPBUF = 0xff;		
    while(!PIR1bits.SSPIF);	// Aguarda a transmissão de 1 byte
    PIR1bits.SSPIF = 0;
    return(SSPBUF);  
}
*/
void main(void) {
    
    printf("Hello World !");
    
    return;
}
