/*
 * File:   main.c
 * Author: dlourenco
 *
 * Created on 28 de Mar?o de 2021, 01:14
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <p24FJ1024GB610.h>
#include "libpic30.h"

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/tmr1.h"
#include "main.h"

#define    FCY    8000000UL

#define SET_VCC2_OUT        TRISAbits.TRISA0
#define SET_VCC_AN_OUT      TRISAbits.TRISA1
#define ENABLE_VCC2         PORTAbits.RA0
#define ENABLE_VCC_AN       PORTAbits.RA1

#define SPIA_SS_TRIS        TRISFbits.TRISF5
#define SPIA_SS_PORT        PORTFbits.RF5
#define SPIB_SS_TRIS        TRISGbits.TRISG13
#define SPIB_SS_PORT        PORTGbits.RG13

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
    
    SPIA_SS_TRIS = 0;           //seleciona pino como output
    SPIA_SS_PORT = 1;           //seleciona SPI_A como slave (setor analogico)
    
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
    
    SPIB_SS_TRIS = 0;           //seleciona pino como output
    SPIB_SS_PORT = 1;           //seleciona SPI_A como slave (EEPROM)
    
    SPI1CON1Lbits.SPIEN = 1;    //habilita modulo SPI
}

void SPI_Write(unsigned char x)
{
    unsigned char data_flush;
    SPI1BUFL = x;			// Copia o dado em SPI1BUFL para transmitir

    while(!SPI1STATLbits.SPIRBF);	// Aguarda a transmiss?o de 1 byte
    SPI1STATLbits.SPIRBF=0;		// Limpa SPIRBF
    data_flush = SPI1BUFL;
}

unsigned char SPI_Read()
{    
    SPI1BUFL = 0xff;		
    while(!SPI1STATLbits.SPIRBF);	// Aguarda a transmiss?o de 1 byte
    SPI1STATLbits.SPIRBF = 0;
    return(SPI1BUFL);  
}

int main() {
    
    SYSTEM_Initialize();
   
    SET_VCC2_OUT = 0;
    ENABLE_VCC2 = 1;
    SET_VCC_AN_OUT = 0;
    ENABLE_VCC_AN = 1;
     
    while(1)
    {
    KBD_SM();
    BATERIA_SM();
    SENSOR_SM();
    DISPLAY_SM();
    }
}
