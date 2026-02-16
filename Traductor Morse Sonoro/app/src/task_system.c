/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file   : task_system.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_system_attribute.h"
#include "task_system_interface.h"
#include "task_GPIO_output_attribute.h"
#include "task_GPIO_output_interface.h"
#include "memoria_morse.h"

/********************** macros and definitions *******************************/
#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_MIN					0ul
#define DEL_SYS_MED					50ul
#define DEL_SYS_MAX					500ul

#define WORDS_PER_MINUTE			240 //ms
#define MORSE_SPACE					7	//dots
#define MORSE_SPACE_CHAR			3	//dots
#define MORSE_SPACE_MORSE			1	//dots

#define DOT_LEN						1
#define LINE_LEN					3


#define VELOCITY1					70
#define VELOCITY2					70
#define VELOCITY3					70

#define RECEIVE_CODE				'.'
#define TRANSMIT_CODE				'#'

#define NULL_MESSAGE				'\0'

/********************** internal data declaration ****************************/
task_system_dta_t task_system_dta =
	{DEL_SYS_MIN, ST_SYS_RECEIVING, ST_SYS_WAITING_CONNECTION, {} , 0 , WORDS_PER_MINUTE , G_TASK_SYS_CNT_INI , false , false};
#define SYSTEM_DTA_QTY	(sizeof(task_system_dta)/sizeof(task_system_dta_t))

/********************** internal functions declaration ***********************/
void task_system_statechart(void);
void velocity_management(task_system_dta_t*);

/********************** internal data definition *****************************/
const char *p_task_system 		= "Task System (System Statechart)";
const char *p_task_system_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_system_cnt;
volatile uint32_t g_task_system_tick_cnt;

/********************** external functions definition ************************/
void task_system_init(void *parameters)
{
	task_system_dta_t 	*p_task_system_dta;
	task_system_st_t	state;
	task_system_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_system_init), p_task_system);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_system), p_task_system_);

	/* Init & Print out: Task execution counter */
	g_task_system_cnt = G_TASK_SYS_CNT_INI;
	LOGGER_INFO("   %s = %lu", GET_NAME(g_task_system_cnt), g_task_system_cnt);

	init_queue_event_task_system();

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_system_dta = &task_system_dta;

	/* Init & Print out: Task execution FSM */
	state = ST_SYS_WAITING_CONNECTION;
	p_task_system_dta->state = state;

	event = EV_SYS_CONNECTION_LOST;
	p_task_system_dta->event = event;

	b_event = false;
	p_task_system_dta->flag = b_event;


	LOGGER_INFO(" ");
	LOGGER_INFO("   %s = %lu   %s = %lu   %s = %s",
				 GET_NAME(state), (uint32_t)state,
				 GET_NAME(event), (uint32_t)event,
				 GET_NAME(b_event), (b_event ? "true" : "false"));
}

void task_system_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_system_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_system_cnt++;

		/* Run Task Statechart */
    	task_system_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_system_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
    }
}

void task_system_statechart(void)
{
	task_system_dta_t *p_task_system_dta;
	char message = NULL_MESSAGE;

	/* Update Task System Data Pointer */
	p_task_system_dta = &task_system_dta;

	if (true == any_event_task_system())
	{
		p_task_system_dta->flag = true;
		p_task_system_dta->event = get_event_task_system();
	}

	switch (p_task_system_dta->state)
	{
		case ST_SYS_WAITING_CONNECTION:


			if(p_task_system_dta->flag == true && p_task_system_dta->event == EV_SYS_CONNECTION_ESTABLISHED){
				p_task_system_dta->state = ST_SYS_RECEIVING;
				put_event_task_GPIO_output(EV_GPIO_XX_ON, ID_LED_RECEIVE);
				put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_LED_TRANSMIT);
			}

			if(p_task_system_dta->flag) {velocity_management(p_task_system_dta);}
			p_task_system_dta->flag = false;
			break;

		case ST_SYS_RECEIVING:

			p_task_system_dta->WPM_tick++;
			if(p_task_system_dta->WPM_tick >= p_task_system_dta->WPM_period){
				p_task_system_dta->WPM_flag = true;
				p_task_system_dta->WPM_tick = 0;
			}

			//RECEIVING SEQUENCE
			if(p_task_system_dta->WPM_flag){
				if(any_message_task_system()){
					message = get_message_task_system();
				}

				put_event_task_GPIO_output(EV_GPIO_XX_PULSE, ID_LED_ERROR);

				if(message == TRANSMIT_CODE){
					p_task_system_dta->state = ST_SYS_TRANSMITTING;
					put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_LED_RECEIVE);
					put_event_task_GPIO_output(EV_GPIO_XX_ON, ID_LED_TRANSMIT);
					put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_LED_ERROR);
					put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_BUZZER);
				}else{

				}
			}
			//RECEIVING SEQUENCE

			if(p_task_system_dta->flag == true && p_task_system_dta->event == EV_SYS_CONNECTION_LOST){
				p_task_system_dta->state = ST_SYS_WAITING_CONNECTION;
				put_event_task_GPIO_output(EV_GPIO_XX_BLINK, ID_LED_RECEIVE);
				put_event_task_GPIO_output(EV_GPIO_XX_BLINK, ID_LED_TRANSMIT);
				put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_LED_ERROR);
				put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_BUZZER);
			}

			p_task_system_dta->WPM_flag = false;

			if(p_task_system_dta->flag) {velocity_management(p_task_system_dta);}
			p_task_system_dta->flag = false;
			break;


		case ST_SYS_TRANSMITTING:

			//TRANSMITTING SEQUENCE
			if(any_message_task_system()){

				message = get_message_task_system();

				if(message == RECEIVE_CODE){
					p_task_system_dta->state = ST_SYS_RECEIVING;
					put_event_task_GPIO_output(EV_GPIO_XX_ON, ID_LED_RECEIVE);
					put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_LED_TRANSMIT);
					put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_LED_ERROR);
					put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_BUZZER);
				} else {
					EEPROM_SymbolToMorse(&p_task_system_dta->morse_character, message);
					p_task_system_dta->morse_character_tick = 0;
					p_task_system_dta->state = ST_SYS_TRANSMITTING_CHAR;
				}
			}
			//TRANSMITTING SEQUENCE

			if(p_task_system_dta->flag == true && p_task_system_dta->event == EV_SYS_CONNECTION_LOST){
				p_task_system_dta->state = ST_SYS_WAITING_CONNECTION;
				put_event_task_GPIO_output(EV_GPIO_XX_BLINK, ID_LED_RECEIVE);
				put_event_task_GPIO_output(EV_GPIO_XX_BLINK, ID_LED_TRANSMIT);
				put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_LED_ERROR);
				put_event_task_GPIO_output(EV_GPIO_XX_OFF, ID_BUZZER);
			}

			p_task_system_dta->WPM_flag = false;

			if(p_task_system_dta->flag) {velocity_management(p_task_system_dta);}
			p_task_system_dta->flag = false;

			break;
		case ST_SYS_TRANSMITTING_CHAR:

			p_task_system_dta->WPM_tick++;
			if(p_task_system_dta->WPM_tick >= p_task_system_dta->WPM_period){
				p_task_system_dta->WPM_flag = true;
				p_task_system_dta->WPM_tick = 0;
			}

			if(p_task_system_dta->WPM_flag){
				if(p_task_system_dta->morse_character_tick %2 && p_task_system_dta->morse_character.len*2 - 1 >  p_task_system_dta->morse_character_tick){

					p_task_system_dta->state = ST_SYS_TRANSMITTING_WAIT;
					p_task_system_dta->tick = MORSE_SPACE_MORSE;
					p_task_system_dta->morse_character_tick++;

				} else if(p_task_system_dta->morse_character.len*2 - 1 >  p_task_system_dta->morse_character_tick){


					if(p_task_system_dta->morse_character.morse_sequence[p_task_system_dta->morse_character_tick/2] == DOT){
						put_event_task_GPIO_output(EV_GPIO_XX_ON , ID_BUZZER);
						p_task_system_dta->state = ST_SYS_TRANSMITTING_WAIT;
						p_task_system_dta->tick = DOT_LEN;
					}else if (p_task_system_dta->morse_character.morse_sequence[p_task_system_dta->morse_character_tick/2] == LINE){
						put_event_task_GPIO_output(EV_GPIO_XX_ON , ID_BUZZER);
						p_task_system_dta->state = ST_SYS_TRANSMITTING_WAIT;
						p_task_system_dta->tick = LINE_LEN;
					} else { //Caso espacio
						put_event_task_GPIO_output(EV_GPIO_XX_OFF , ID_BUZZER);
						p_task_system_dta->state = ST_SYS_TRANSMITTING_WAIT;
						p_task_system_dta->tick = MORSE_SPACE - 1;
					}
					p_task_system_dta->morse_character_tick++;
				}else if (p_task_system_dta->morse_character.len*2 - 1 ==  p_task_system_dta->morse_character_tick){
					p_task_system_dta->state = ST_SYS_TRANSMITTING_WAIT;
					p_task_system_dta->tick = MORSE_SPACE_MORSE;
					p_task_system_dta->morse_character_tick ++;
				} else {
					p_task_system_dta->state = ST_SYS_TRANSMITTING;
					p_task_system_dta->tick = 0;
					p_task_system_dta->morse_character_tick = 0;
				}
				p_task_system_dta->WPM_flag = false;
			}

			if(p_task_system_dta->flag) {velocity_management(p_task_system_dta);}
			p_task_system_dta->flag = false;
			break;

		case ST_SYS_TRANSMITTING_WAIT:
			p_task_system_dta->WPM_tick++;
			if(p_task_system_dta->WPM_tick >= p_task_system_dta->WPM_period){
				p_task_system_dta->WPM_flag = true;
				p_task_system_dta->WPM_tick = 0;
			}

			if(p_task_system_dta->WPM_flag){
				p_task_system_dta->tick--;
				p_task_system_dta->WPM_flag = false;
			}
			if(p_task_system_dta->tick == 0){
				put_event_task_GPIO_output(EV_GPIO_XX_OFF , ID_BUZZER);
				p_task_system_dta->state = ST_SYS_TRANSMITTING_CHAR;
			}

			if(p_task_system_dta->flag) {velocity_management(p_task_system_dta);}
			p_task_system_dta->flag = false;
			break;
		default:
			break;
	}
}

void velocity_management(task_system_dta_t* p_task_system_dta){
	switch(p_task_system_dta->event){
		case EV_SYS_VELOCITY1_ON:
			p_task_system_dta->WPM_period += VELOCITY1;
			break;
		case EV_SYS_VELOCITY1_OFF:
			p_task_system_dta->WPM_period -= VELOCITY1;
			break;
		case EV_SYS_VELOCITY2_ON:
			p_task_system_dta->WPM_period += VELOCITY2;
			break;
		case EV_SYS_VELOCITY2_OFF:
			p_task_system_dta->WPM_period -= VELOCITY2;
			break;
		case EV_SYS_VELOCITY3_ON:
			p_task_system_dta->WPM_period += VELOCITY3;
			break;
		case EV_SYS_VELOCITY3_OFF:
			p_task_system_dta->WPM_period -= VELOCITY3;
			break;
		default:
			break;
	}
}
/********************** end of file ******************************************/
