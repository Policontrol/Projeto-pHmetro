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
}
