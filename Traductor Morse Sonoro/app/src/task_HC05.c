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
 * @file   : task_HC05.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/*	This task handles bidirectional serial communication between two devices.
 * 	Longer buffers are to be implemented, as the current implementation handles
 * 	byte per byte, with no buffering support (inferring that this scenario is
 * 	the result of mismatching transfer rates).
 * 	Transfer frequency is that of Systick.
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
#include "task_hc05_attribute.h"
#include "task_HC05_interface.h"
#include "task_system_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_HC05_CNT_INIT			0ul
#define G_TASK_HC05_TICK_CNT_INI		0ul

#define HC05_ALLOW_FULLDUPLEX			false
#define DEL_HC05_MIN					0ul
#define DEL_HC05_MAX					50ul
#define HC05_TIMEOUT					10

/********************** internal data declaration ****************************/
const task_hc05_cfg_t task_hc05_cfg_list[] = {
		{ID_HC05 , &huart1 , UART_RX_PORT , UART_RX_PIN , UART_TX_PORT , UART_TX_PIN
		, UART_STATUS_PORT , UART_STATUS_PIN , GPIO_PIN_SET , DEL_HC05_MAX , EV_SYS_CONNECTION_ESTABLISHED
		, EV_SYS_CONNECTION_LOST }
};

#define HC05_CFG_QTY	(sizeof(task_hc05_cfg_list)/sizeof(task_hc05_cfg_t))

task_hc05_dta_t task_hc05_dta_list[] = {
		{DEL_HC05_MIN , ST_HC05_DISCONNECTED , EV_HC05_DISCONNECTED}
};

#define HC05_DTA_QTY	(sizeof(task_hc05_dta_list)/sizeof(task_hc05_dta_t))

/********************** internal functions declaration ***********************/
void task_hc05_statechart(void);

/********************** internal data definition *****************************/
const char *p_task_hc05 		= "Task HC_05 (HC_05 Statechart)";
const char *p_task_hc05_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_hc05_cnt;
volatile uint32_t g_task_hc05_tick_cnt;

/********************** external functions definition ************************/
void task_hc05_init(void *parameters)
{
	uint32_t index;
	task_hc05_dta_t *p_task_hc05_dta;
	const task_hc05_cfg_t *p_task_hc05_cfg;
	task_hc05_st_t state;
	task_hc05_ev_t event;

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_hc05_init), p_task_hc05);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_hc05), p_task_hc05_);

	/* Init & Print out: Task execution counter */
	g_task_hc05_cnt = G_TASK_HC05_CNT_INIT;
	LOGGER_INFO("   %s = %lu", GET_NAME(g_task_hc05_cnt), g_task_hc05_cnt);

	init_queue_messages_task_HC05();

	for (index = 0; HC05_DTA_QTY > index; index++)
	{
		/* Update Task HC05 Data Pointer */
		p_task_hc05_dta = &task_hc05_dta_list[index];
		p_task_hc05_cfg = &task_hc05_cfg_list[index];

		/* Init & Print out: Index & Task execution FSM */
		state = ST_HC05_DISCONNECTED;
		p_task_hc05_dta->state = state;

		event = EV_HC05_DISCONNECTED;
		p_task_hc05_dta->event = event;

		p_task_hc05_dta->rx_flag = 0;
		p_task_hc05_dta->tx_flag = 0;

		HAL_UART_Receive_IT(p_task_hc05_cfg->uart, &p_task_hc05_dta->rx_buffer, 1);

		LOGGER_INFO(" ");
		LOGGER_INFO("   %s = %lu   %s = %lu   %s = %lu",
				    GET_NAME(index), index,
					GET_NAME(state), (uint32_t)state,
					GET_NAME(event), (uint32_t)event);
	}
}

void task_hc05_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_HC05_TICK_CNT_INI < g_task_hc05_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_hc05_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_hc05_cnt++;

		/* Run Task HC05 Statechart */
    	task_hc05_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_HC05_TICK_CNT_INI < g_task_hc05_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_hc05_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
    }
}

/********************** internal functions definition ************************/
void task_hc05_statechart(void)
{
	uint32_t index;
	const task_hc05_cfg_t *p_task_hc05_cfg;
	task_hc05_dta_t *p_task_hc05_dta;

	for (index = 0; HC05_DTA_QTY > index; index++)
	{
		/* Update Task HC05 Configuration & Data Pointer */
		p_task_hc05_cfg = &task_hc05_cfg_list[index];
		p_task_hc05_dta = &task_hc05_dta_list[index];

		if(any_tx_message_task_HC05()){
			p_task_hc05_dta->tx_flag = 1;
		}


		if (p_task_hc05_cfg->connected == HAL_GPIO_ReadPin(p_task_hc05_cfg->pin_status_port, p_task_hc05_cfg->pin_status))
		{
			if(p_task_hc05_dta->rx_flag != 0 && p_task_hc05_dta->tx_flag != 0)
				p_task_hc05_dta->event = EV_HC05_FULLDUPLEX;
			else if(p_task_hc05_dta->rx_flag != 0)
				p_task_hc05_dta->event = EV_HC05_RX_BUFFER;
			else if(p_task_hc05_dta->tx_flag)
				p_task_hc05_dta->event = EV_HC05_TX_BUFFER;
			else
				p_task_hc05_dta->event = EV_HC05_CONNECTED;
		}
		else
		{
			p_task_hc05_dta->event = EV_HC05_DISCONNECTED;
		}


		switch (p_task_hc05_dta->state)
		{
			case ST_HC05_DISCONNECTED:

				if(p_task_hc05_dta->event == EV_HC05_CONNECTED){
					put_event_task_system(p_task_hc05_cfg->connection_established);
					p_task_hc05_dta->state = ST_HC05_CONNECTED;
				}
			break;

			case ST_HC05_CONNECTED:

				if(p_task_hc05_dta->event == EV_HC05_FULLDUPLEX){
					if(HC05_ALLOW_FULLDUPLEX)
						p_task_hc05_dta->state = ST_HC05_RECEIVING;
					else
						p_task_hc05_dta->state = ST_HC05_ERROR;
				}
				else if (p_task_hc05_dta->event == EV_HC05_RX_BUFFER)
					p_task_hc05_dta->state = ST_HC05_RECEIVING;
				else if (p_task_hc05_dta->event == EV_HC05_TX_BUFFER)
					p_task_hc05_dta->state = ST_HC05_SENDING;

				if(p_task_hc05_dta->event == EV_HC05_DISCONNECTED){
					p_task_hc05_dta->state = ST_HC05_DISCONNECTED;
					put_event_task_system(p_task_hc05_cfg->connection_lost);
				}
			break;

			case ST_HC05_RECEIVING:
				p_task_hc05_dta->rx_flag = 0;

				//RX_BUFFER NOT EMPTY
				put_message_rx_system(p_task_hc05_dta->rx_buffer);

				p_task_hc05_dta->state = ST_HC05_CONNECTED;
				if(p_task_hc05_dta->event == EV_HC05_TX_BUFFER)
					p_task_hc05_dta->state = ST_HC05_SENDING;
				if(p_task_hc05_dta->event == EV_HC05_DISCONNECTED){
					p_task_hc05_dta->state = ST_HC05_DISCONNECTED;
					put_event_task_system(p_task_hc05_cfg->connection_lost);
				}
			break;

			case ST_HC05_SENDING:

				//TX BUFFER NOT EMPTY
				p_task_hc05_dta->tx_buffer = get_tx_message_task_HC05();

				HAL_UART_Transmit(p_task_hc05_cfg->uart, &p_task_hc05_dta->tx_buffer, 1, HC05_TIMEOUT);
				p_task_hc05_dta->tx_flag = 0;

				p_task_hc05_dta->state = ST_HC05_CONNECTED;
				if(p_task_hc05_dta->event == EV_HC05_RX_BUFFER)
					p_task_hc05_dta->state = ST_HC05_RECEIVING;
				if(p_task_hc05_dta->event == EV_HC05_DISCONNECTED){
					p_task_hc05_dta->state = ST_HC05_DISCONNECTED;
					put_event_task_system(p_task_hc05_cfg->connection_lost);
				}
			break;

			case ST_HC05_ERROR:
			break;

			default:

				p_task_hc05_dta->tick  = DEL_HC05_MIN;
				p_task_hc05_dta->state = ST_HC05_ERROR;
				p_task_hc05_dta->event = EV_HC05_DISCONNECTED;

				break;
		}
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	uint32_t index;
	task_hc05_dta_t *p_task_hc05_dta;
	const task_hc05_cfg_t *p_task_hc05_cfg;

	for (index = 0; HC05_DTA_QTY > index; index++)
	{
		p_task_hc05_dta = &task_hc05_dta_list[index];
		p_task_hc05_cfg = &task_hc05_cfg_list[index];
		if(huart->Instance == p_task_hc05_cfg->uart->Instance){
			p_task_hc05_dta->rx_flag = 1;
			HAL_UART_Receive_IT(p_task_hc05_cfg->uart, &p_task_hc05_dta->rx_buffer, 1);
			break;
		}
	}
}
/*
void task_hc05_transmit( task_hc05_id_t id ,uint8_t data){
	uint32_t index;
	task_hc05_dta_t *p_task_hc05_dta;
	const task_hc05_cfg_t *p_task_hc05_cfg;
	for (index = 0; HC05_DTA_QTY > index; index++)
	{
		p_task_hc05_dta = &task_hc05_dta_list[index];
		p_task_hc05_cfg = &task_hc05_cfg_list[index];
		if(p_task_hc05_cfg->identifier == id){
			p_task_hc05_dta->tx_flag = 1;
			p_task_hc05_dta->tx_buffer = data;
			break;
		}
	}
}*/
/********************** end of file ******************************************/
