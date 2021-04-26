/*
 * File:   main.c
 * Author: dlourenco
 *
 * Created on 28 de Março de 2021, 01:14
 */

#include <stdio.h>
#include <stdlib.h>
#include <p24FJ1024GB610.h>
#include "main.h"

#define SPIA_SS_TRIS      TRISFbits.TRISF5
#define SPIA_SS_PORT      PORTFbits.RF5

#define SPIB_SS_TRIS      TRISGbits.TRISG13
#define SPIB_SS_PORT      PORTGbits.RG13

void SPI_Init_Master_A()
{
    SPI1CON1Lbits.SPIEN = 0;    //desabilita modulo SPI
    SPI1CON1Lbits.SPISIDL = 0;  //modulo continua operando mesmo com CPU inativa
    SPI1BUFH = 0;               //limpa o buffer do SPI
    SPI1BUFL = 0;               //limpa o buffer do SPI
    SPI1CON1Lbits.SSEN = 0;     //SS sera setado por um pino e nao pelo macro
    SPI1CON1Lbits.MSTEN = 1;    //seleciona master mode
    SPI1CON1Lbits.DISSDI = 0;   //habilita SDI
    SPI1CON1Lbits.DISSDO = 0;   //habilita SDO
    SPI1CON1Lbits.DISSCK = 0;   //habilita clock interno do SPI
    
    SPIA_SS_TRIS = 0;           //seleciona SPI_A como slave (setor analogico)
    SPIA_SS_PORT = 1;
    
    SPI1CON1Lbits.SPIEN = 1;    //habilita modulo SPI
}

void SPI_Init_Master_B()
{
    SPI1CON1Lbits.SPIEN = 0;    //desabilita modulo SPI
    SPI1CON1Lbits.SPISIDL = 0;  //modulo continua operando mesmo com CPU inativa
    SPI1BUFH = 0;               //limpa o buffer do SPI
    SPI1BUFL = 0;               //limpa o buffer do SPI
    SPI1CON1Lbits.SSEN = 0;     //SS sera setado por um pino e nao pelo macro
    SPI1CON1Lbits.MSTEN = 1;    //seleciona master mode
    SPI1CON1Lbits.DISSDI = 0;   //habilita SDI
    SPI1CON1Lbits.DISSDO = 0;   //habilita SDO
    SPI1CON1Lbits.DISSCK = 0;   //habilita clock interno do SPI
    
    SPIB_SS_TRIS = 0;           //seleciona SPI_A como slave (EEPROM)
    SPIB_SS_PORT = 1;
    
    SPI1CON1Lbits.SPIEN = 1;    //habilita modulo SPI
}

void SPI_Write(unsigned char x)
{
    unsigned char data_flush;
    SPI1BUFL = x;			// Copia o dado em SPI1BUFL para transmitir

    while(!SPI1STATLbits.SPIRBF);	// Aguarda a transmissão de 1 byte
    SPI1STATLbits.SPIRBF=0;		// Limpa SPIRBF
    data_flush = SPI1BUFL;
}

unsigned char SPI_Read()
{    
    SPI1BUFL = 0xff;		
    while(!SPI1STATLbits.SPIRBF);	// Aguarda a transmissão de 1 byte
    SPI1STATLbits.SPIRBF = 0;
    return(SPI1BUFL);  
}

int main() {
    
    while(1)
    {
    SPI_Init_Master_A();
    SPI_Read();
    SPI_Init_Master_B();
    SPI_Read();
    }
}
