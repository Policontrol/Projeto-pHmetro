/*
 * File:   display.c
 * Author: dlour
 *
 * Created on 25 de Junho de 2021, 18:31
 */

#include "display.h"
#include "kbd.h"

//extern enum tela_t tela;
extern uint8_t posicao;
extern uint8_t leitura_bateria;
extern bool status_alim;
extern uint32_t sensor_read;

#define GLCD_CS1 LATDbits.LATD11
/** The reset pin (this is required and should not be tied high) */
#define GLCD_RESET LATFbits.LATF1
/** The A0 pin, which selects command or data mode */
#define GLCD_A0 LATBbits.LATB15
/** The clock pin */
#define GLCD_SCL LATEbits.LATE6
/** The data pin */
#define GLCD_SDA LATEbits.LATE7

#define MAX_PIXEL_X             128
#define MAX_PIXEL_Y             64

#define LCD_FB_SIZE             1024
unsigned char buffer[LCD_FB_SIZE];

#define H_BYTE_CNT      43

void DataReadOut()
{
    TRISDbits.TRISD11 = 0;      //CSB
    TRISBbits.TRISB15 = 0;     //A0
    TRISDbits.TRISD4 = 0;      //RWR
    TRISDbits.TRISD5 = 0;      //ERD
    
    PORTDbits.RD11 = 0;
    PORTBbits.RB15 = 1;     //A0
    PORTDbits.RD4 = 1;      //RWR
    PORTDbits.RD5 = 0;      //ERD
}

void DataWrite()
{
    TRISDbits.TRISD11 = 0;      //CSB
    TRISBbits.TRISB15 = 0;     //A0
    TRISDbits.TRISD4 = 0;      //RWR
    TRISDbits.TRISD5 = 0;      //ERD
    
    PORTDbits.RD11 = 0;     //CSB
    PORTBbits.RB15 = 1;     //A0
    PORTDbits.RD4 = 0;      //RWR
    PORTDbits.RD5 = 1;      //ERD
}

void TurnDisplayOn()
{
    TRISDbits.TRISD11 = 0;      //CSB
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0; 
    //TRISE = 0x000;
    TRISFbits.TRISF1 = 0;
    Nop();

//    LATFbits.LATF1 = 0;      //LCD_RST
//    Nop();
//    LATFbits.LATF1 = 1;      //LCD_RST
//    Nop();
    
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD11 = 0;     //CSB
    LATDbits.LATD4 = 0;      //RWR
    LATEbits.LATE7 = 1;
    LATEbits.LATE6 = 0;
    LATEbits.LATE5 = 1;
    LATEbits.LATE4 = 0;
    LATEbits.LATE3 = 1;
    LATEbits.LATE2 = 1;
    LATEbits.LATE1 = 1;
    LATEbits.LATE0 = 1;
    Nop();                  //f = 8MHz -> t = 125ns
    //LATDbits.LATD11 = 1;     //CSB
    LATDbits.LATD4 = 1;      //RWR
    Nop();                  //f = 8MHz -> t = 125ns
}

void SetLcdBias(uint8_t BS)
{
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR
    LATEbits.LATE7 = 1;
    LATEbits.LATE6 = 0;
    LATEbits.LATE5 = 1;
    LATEbits.LATE4 = 0;
    LATEbits.LATE3 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE1 = 1;
    
    if(BS == 0)
        LATEbits.LATE0 = 0; //1/9
    
    else
        LATEbits.LATE0 = 1; //1/7
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
}

void SetLcdSegDirection(uint8_t MX)
{
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR
    LATEbits.LATE7 = 1;
    LATEbits.LATE6 = 0;
    LATEbits.LATE5 = 1;
    LATEbits.LATE4 = 0;
    LATEbits.LATE3 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE1 = 0;
    
    if(MX == 0)
        LATEbits.LATE0 = 0; //Normal direction
    
    else
        LATEbits.LATE0 = 1; //Reverse Direction
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
}

void SetLcdComDirection(uint8_t MY)
{
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR
    LATEbits.LATE7 = 1;
    LATEbits.LATE6 = 1;
    LATEbits.LATE5 = 0;
    LATEbits.LATE4 = 0;
    LATEbits.LATE3 = 0;
    
    if(MY == 0)
        LATEbits.LATE3 = 0; //Normal direction
    
    else
        LATEbits.LATE3 = 1; //Reverse Direction
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
}

void SetLcdRegulationRatio(double RR)
{
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR
    LATEbits.LATE7 = 0;
    LATEbits.LATE6 = 0;
    LATEbits.LATE5 = 1;
    LATEbits.LATE4 = 0;
    LATEbits.LATE3 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE0 = 1;
    
    /*
    switch(RR)
    {
        case 3.0:
            LATEbits.LATE2 = 0;
            LATEbits.LATE1 = 0;
            LATEbits.LATE0 = 0;
        break;
        
        case 3.5:
            LATEbits.LATE2 = 0;
            LATEbits.LATE1 = 0;
            LATEbits.LATE0 = 1;
        break;
        
        case 4.0:
            LATEbits.LATE2 = 0;
            LATEbits.LATE1 = 1;
            LATEbits.LATE0 = 0;
        break;
        
        case 4.5:
            LATEbits.LATE2 = 0;
            LATEbits.LATE1 = 1;
            LATEbits.LATE0 = 1;
        break;
        
        case 5.0:
            LATEbits.LATE2 = 1;
            LATEbits.LATE1 = 0;
            LATEbits.LATE0 = 0;
        break;
        
        case 5.5:
            LATEbits.LATE2 = 1;
            LATEbits.LATE1 = 0;
            LATEbits.LATE0 = 1;
        break;
        
        case 6.0:
            LATEbits.LATE2 = 1;
            LATEbits.LATE1 = 1;
            LATEbits.LATE0 = 0;
        break;
        
        case 6.5:
            LATEbits.LATE2 = 1;
            LATEbits.LATE1 = 1;
            LATEbits.LATE0 = 1;
        break;
        
        default:    //3.5 default
            LATEbits.LATE2 = 0;
            LATEbits.LATE1 = 0;
            LATEbits.LATE0 = 1;
    }
    */
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
}

void SetLcdEV()
{
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR
    LATEbits.LATE7 = 1;
    LATEbits.LATE6 = 0;
    LATEbits.LATE5 = 0;
    LATEbits.LATE4 = 0;
    LATEbits.LATE3 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE0 = 1;
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
    
    Nop();
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR
    Nop();
//    LATEbits.LATE7 = 0;
//    LATEbits.LATE6 = 0;
//    LATEbits.LATE5 = 1;
//    LATEbits.LATE4 = 0;
//    LATEbits.LATE3 = 0;
//    LATEbits.LATE2 = 0;
//    LATEbits.LATE1 = 0;
//    LATEbits.LATE0 = 0;
    LATE = 0x20;
    //EV = 32  
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
}

void SetLcdPowerControl()
{
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR
    LATEbits.LATE7 = 0;
    LATEbits.LATE6 = 0;
    LATEbits.LATE5 = 1;
    LATEbits.LATE4 = 0;
    LATEbits.LATE3 = 1;
    LATEbits.LATE2 = 1;
    LATEbits.LATE1 = 1;
    LATEbits.LATE0 = 1;
    //booster, regulator and follower ON  
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
}

void LcdReadStatus()
{
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISDbits.TRISD5 = 0;      //ERD
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 1;
    TRISEbits.TRISE5 = 1;
    TRISEbits.TRISE6 = 1;
    TRISEbits.TRISE7 = 0;
    
    LATDbits.LATD5 = 0;      //ERD
    LATBbits.LATB15 = 0;     //A0  
    LATEbits.LATE7 = 0;
//    LATEbits.LATE6 = 0;
//    LATEbits.LATE5 = 1;
//    LATEbits.LATE4 = 0;
    LATEbits.LATE3 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE0 = 0;
//    LATDbits.LATD4 = 1;      //RWR
    
    LATDbits.LATD5 = 1;      //ERD
}

void SetLcdSetStartLine()
{
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR
    LATEbits.LATE7 = 0;
    LATEbits.LATE6 = 1;
    LATEbits.LATE5 = 0;
    LATEbits.LATE4 = 0;
    LATEbits.LATE3 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE0 = 0;
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
}

void SetLcdSetPageAddress(unsigned char page)
{
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
//    LATBbits.LATB15 = 0;     //A0
//    LATDbits.LATD4 = 0;      //RWR
//    LATEbits.LATE7 = 1;
//    LATEbits.LATE6 = 0;
//    LATEbits.LATE5 = 1;
//    LATEbits.LATE4 = 1;
//    LATEbits.LATE3 = 0;
//    LATEbits.LATE2 = 0;
//    LATEbits.LATE1 = 0;
//    LATEbits.LATE0 = 0;
    
    LATE = page;
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
}

void SetLcdSetColumnAddress()
{
    TRISBbits.TRISB15 = 0;      //A0
    TRISDbits.TRISD4 = 0;       //RWR
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;
    
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR
    LATEbits.LATE7 = 0;
    LATEbits.LATE6 = 0;
    LATEbits.LATE5 = 0;
    LATEbits.LATE4 = 1;
    LATEbits.LATE3 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE0 = 0;
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
    Nop();
    
    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR
    LATEbits.LATE7 = 0;
    LATEbits.LATE6 = 0;
    LATEbits.LATE5 = 0;
    LATEbits.LATE4 = 0;
    LATEbits.LATE3 = 0;
    LATEbits.LATE2 = 0;
    LATEbits.LATE1 = 0;
    LATEbits.LATE0 = 0;
    
    Nop();
    LATDbits.LATD4 = 1;      //RWR
}

void DisplayInitialize()
{
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD11 = 0;      //CSB

    LATDbits.LATD11 = 0;     //CSB
//    __delay_ms(2);
    __delay32(16000);
    
    LATFbits.LATF1 = 0;      //LCD_RST
//    __delay_us(10);
    __delay32(8000);
    LATFbits.LATF1 = 1;      //LCD_RST
//    __delay_us(10);
    __delay32(8000);
    
    SetLcdBias(0);
    __delay32(8000);
//    TurnDisplayOn();
//    __delay32(8000);
    SetLcdSegDirection(0);
    __delay32(8000);
    SetLcdComDirection(1);
    __delay32(8000);
    
    SetLcdRegulationRatio(3.5);
    __delay32(8000);
    SetLcdEV();
    __delay32(8000);
    
    SetLcdPowerControl();
//    __delay_ms(5);
    __delay32(40000);
    
    SetLcdSetStartLine();
    __delay32(8000);
    SetLcdSetPageAddress(0xB0);
    __delay32(8000);
    SetLcdSetColumnAddress();
    __delay32(40000);
    
    TurnDisplayOn();
    
//    LATDbits.LATD11 = 1;     //CSB
}

void TurnAllPixelsOn()
{
    TRISBbits.TRISB15 = 0;
    TRISDbits.TRISD4 = 0;
    TRISEbits.TRISE0 = 0;
    TRISEbits.TRISE1 = 0;
    TRISEbits.TRISE2 = 0;
    TRISEbits.TRISE3 = 0;
    TRISEbits.TRISE4 = 0;
    TRISEbits.TRISE5 = 0;
    TRISEbits.TRISE6 = 0;
    TRISEbits.TRISE7 = 0;   
    //TRISE = 0x000;
    Nop();

    LATBbits.LATB15 = 0;     //A0
    LATDbits.LATD4 = 0;      //RWR

//    LATEbits.LATE7 = 1;
//    LATEbits.LATE6 = 0;
//    LATEbits.LATE5 = 1;
//    LATEbits.LATE4 = 0;
//    LATEbits.LATE3 = 0;
//    LATEbits.LATE2 = 1;
//    LATEbits.LATE1 = 0;
//    LATEbits.LATE0 = 1;
    
    LATE = 0xA5;
    Nop();                  //f = 8MHz -> t = 125ms


    LATDbits.LATD4 = 1;      //RWR
    Nop();                  //f = 8MHz -> t = 125ms    
    
//    LATDbits.LATD4 = 0;      //RWR
//    LATE = 0xA7;
//    Nop();
//    LATDbits.LATD4 = 1;      //RWR
//    Nop();                  //f = 8MHz -> t = 125ms
    
}

void PixelSetFunction(UG_S16 x , UG_S16 y ,UG_COLOR c)
{
//    unsigned int pos;
//    unsigned char b;
//    
//    pos = (x / 3) + y * H_BYTE_CNT;
//    
//    if(x > MAX_PIXEL_X || y > MAX_PIXEL_Y || x < 0 || y < 0) return;
    
    if(c == 0)
        buffer[x + ((y/8) * 128)] &= ~(1 << (y%8));  // erase pixel
    else
        buffer[x + ((y/8) * 128)] |= (1 << (y%8));   // set pixel
    
//   switch( x%3 )
//   {
//      case 2: b = 0x03; break;
//      case 1: b = 0x81; break;
//      case 0: b = 0xC0; break;
//   }
//   
//    
//   if ( !c )
//   {
//      buffer[ pos ] |= b;
//   }
//   else
//   {
//      buffer[ pos ] &= ~b;
//   }
}

enum {LCD_NORMAL, LCD_XOR};

unsigned int draw_mode;

void pixel(int x, int y, int color)
{
    draw_mode = LCD_NORMAL;
        // first check parameter
    if(x > MAX_PIXEL_X || y > MAX_PIXEL_Y || x < 0 || y < 0) return;
 
    if(draw_mode == LCD_NORMAL) {
        if(color == 0)
            buffer[x + ((y/8) * 128)] &= ~(1 << (y%8));  // erase pixel
        else
            buffer[x + ((y/8) * 128)] |= (1 << (y%8));   // set pixel
    } else { // XOR mode
        if(color == 1)
            buffer[x + ((y/8) * 128)] ^= (1 << (y%8));   // xor pixel
    }
}

void UpdateLCD()
{   
    int page;
    int i;
    for (page = 0; page < 8; page++) 
    {
//        SetLcdSetStartLine();
//        __delay32(8000);
        SetLcdSetColumnAddress();
        __delay32(8000);
        SetLcdSetPageAddress(0xB0|page);
        __delay32(8000);
        LATBbits.LATB15 = 1;     //A0
//        LATDbits.LATD4 = 0;      //RWR

        for(i=page*128; i<(page*128+128); i++) 
        {
//            LATDbits.LATD4 = 0;      //RWR
            LATE = buffer[i];
//            if(page == 2)
//            {
//                LATE = 0xFF;
//            }
//            else
//                LATE = 0x00;
            
            __delay32(8000);
//            LATDbits.LATD4 = 1;      //RWR
        }
    }
    
}

/*
 * 
 * SPI functions
 * 
void glcd_data(unsigned char data)
{    
    int i;
    
    GLCD_A0 = 1;
    
    GLCD_CS1 = 0;
    
    for (i = 0; i < 8; i++)
    {
        if(data & 0x80)
        {
            GLCD_SDA = 1;
        }
        
        else
            GLCD_SDA = 0;
        
        GLCD_SCL = 1;
        GLCD_SCL = 0;
        
        data <<= 1;      
    }
    
    GLCD_CS1 = 1;
}

void glcd_command(char command)
{
    int i;
    
    GLCD_A0 = 1;
    
    GLCD_CS1 = 0;
    
    for (i = 0; i < 8; i++)
    {
        if(command & 0x80)
        {
            GLCD_SDA = 1;
        }
        
        else
        {
            GLCD_SDA = 0;
        }
        
        GLCD_SCL = 1;
        GLCD_SCL = 0;
        
        command <<= 1;      
    }
    
    GLCD_CS1 = 1;
}

void glcd_init()
{
    TRISDbits.TRISD11 = 0;      //CSB
    TRISBbits.TRISB15 = 0;      //A0
    TRISFbits.TRISF1 = 0;       //RST
    TRISEbits.TRISE6 = 0;       //SCL
    TRISEbits.TRISE7 = 0;       //SDA
    
    GLCD_CS1 = 0 ;
    
    GLCD_RESET = 0;
    __delay32(75000);   //50ms
    
    GLCD_RESET = 1;
    
    glcd_command(0b10100010);   //BIAS 1/9
    __delay32(8000);
    
    glcd_command(0b10100000);   //SEG Direction
    __delay32(8000);
    
    glcd_command(0b11000000);   //COM Direction
    __delay32(8000);
    
    glcd_command(0b00100001);   //Regulation Ratio
    __delay32(8000);
    
    glcd_command(0b10000001);   //EV
    Nop();
    glcd_command(32);
    __delay32(8000);
    
    glcd_command(0b00101111);   //Power Control
    __delay32(8000);
    
    glcd_command(0b01000000);   //Start Line
    __delay32(8000);
    
    glcd_command(0xB0);   //Page Address
    __delay32(8000);
    
    glcd_command(0b00010000);   //Column Address
    Nop();
    glcd_command(0b00000000);
    __delay32(8000);
    
    glcd_command(0b10101111);   //Display ON
    __delay32(8000);
    
    GLCD_CS1 = 1;
}
*/
void DISPLAY_SM()
{
//    	switch(tela)
//	{
//		case PRINCIPAL:
//			//Atualiza leitura do sensor usando dados da variavel sensor_read
//			//Show_Principal(sensor_read, bateria);
//			break;
//			
//		case MENU:
//			//Show_Menu(posição);
//			break;
//			
//		case CALI:
//			//Show_Cali(...);
//			break;
//		
//		case MED:
//			//Show_Med(...);
//			break;
//			
//		case CONF:
//			//Show_Conf(...)
//			break;
//		default:
//			break;
//	}
    
//    UG_GUI gui;
//    UG_S16 ret;
    
    DisplayInitialize();
    __delay32(3000000);
    TurnAllPixelsOn();
    __delay32(1000000);    //1.5s   
    
//    UG_Init(&gui, PixelSetFunction, MAX_PIXEL_X, MAX_PIXEL_Y);
//    UG_FontSelect(&FONT_6X8);
//    UG_ConsoleSetArea(0, 0, MAX_PIXEL_X, MAX_PIXEL_Y);
//    UG_SetBackcolor(C_WHITE);
//    UG_SetForecolor(C_BLACK);
//    UG_PutString(0, 0, "Hello World !") ;
//    UG_DrawLine(0, 0, 100, 50, C_BLACK);
//    UG_Update();
//    UpdateLCD();  
}
