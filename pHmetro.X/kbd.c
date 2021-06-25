/*
 * File:   kbd.c
 * Author: dlour
 *
 * Created on 25 de Junho de 2021, 18:24
 */

#include "kbd.h"
     
evento_t evento = 0;
tela_t tela = PRINCIPAL;
tela_t map_tela[3] = {CALI, MED, CONF};
uint8_t posicao = 0;

void KBD_SM()
{
   // Checar os botões do teclado e agir de acordo
      	switch (evento){
		case SETA_UP:
		if (tela == TELA_MENU)
			//posicao = (posicao == MAX ? posicao: posicao + 1);
			break;
		case SETA_DOWN:
		if (tela == TELA_MENU)
			//posicao = (posicao == MIN ? posicao: posicao - 1);
			break;
		case ENTER:
		if (tela == TELA_MENU)
			tela = map_tela[posicao];
			break;
		case BUT_MENU:
		if (tela == PRINCIPAL)
			tela = TELA_MENU;
		else if (tela == TELA_MENU)
			tela = PRINCIPAL;
			break;
		default:
			break;
	}   
}
