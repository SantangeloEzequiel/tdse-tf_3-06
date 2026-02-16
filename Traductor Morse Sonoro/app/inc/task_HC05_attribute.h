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
 * @file   : task_hc05_attribute.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef TASK_INC_TASK_HC05_ATTRIBUTE_H_
#define TASK_INC_TASK_HC05_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "task_system_attribute.h"
/********************** macros ***********************************************/

/********************** typedef **********************************************/

/* Events to excite Task HC-05 */
typedef enum task_hc05_ev {EV_HC05_RX_BUFFER,
							EV_HC05_TX_BUFFER,
							EV_HC05_FULLDUPLEX,
							EV_HC05_DISCONNECTED,
							EV_HC05_CONNECTED} task_hc05_ev_t;

/* States of Task HC-05 */
typedef enum task_hc05_st {ST_HC05_SENDING,
							 ST_HC05_RECEIVING,
							 ST_HC05_DISCONNECTED,
							 ST_HC05_CONNECTED,
							 ST_HC05_ERROR} task_hc05_st_t;

/* Identifier of Task HC-05 */
typedef enum task_hc05_id {ID_HC05} task_hc05_id_t;

typedef struct
{
	task_hc05_id_t			identifier;
	UART_HandleTypeDef *	uart;
	GPIO_TypeDef *			pin_rx_port;
	uint16_t				pin_rx;
	GPIO_TypeDef *			pin_tx_port;
	uint16_t				pin_tx;
	GPIO_TypeDef *			pin_status_port;
	uint16_t				pin_status;
	GPIO_PinState			connected;
	uint32_t				tick_max;
	task_system_ev_t		connection_established;
	task_system_ev_t		connection_lost;
} task_hc05_cfg_t;

typedef struct
{
	uint32_t			tick;
	task_hc05_st_t		state;
	task_hc05_ev_t		event;
	volatile uint8_t	tx_flag;
	volatile uint8_t	rx_flag;
	uint8_t				tx_buffer;
	uint8_t				rx_buffer;
} task_hc05_dta_t;

/********************** external data declaration ****************************/
extern task_hc05_dta_t task_hc05_dta_list[];
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_HC05_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
