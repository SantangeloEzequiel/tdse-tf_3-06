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
 * @file   : task_GPIO_output_attribute.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef TASK_INC_TASK_GPIO_OUTPUT_ATTRIBUTE_H_
#define TASK_INC_TASK_GPIO_OUTPUT_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/
/* Actuator Statechart - State Transition Table */
/* 	------------------------+-----------------------+-----------------------+-----------------------+------------------------
 * 	| Current               | Event                 |                       | Next                  |                       |
 * 	| State                 | (Parameters)          | [Guard]               | State                 | Actions               |
 * 	|=======================+=======================+=======================+=======================+=======================|
 * 	| INICIAL               |                       |                       | ST_GPIO_XX_OFF         | GPIO = GPIO_OFF         |
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_GPIO_XX_OFF         | EV_GPIO_XX_ON          |                       | ST_GPIO_XX_ON          | GPIO = GPIO_ON          |
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|                       |-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_GPIO_XX_BLINK       |                       | ST_GPIO_XX_BLINK_ON    | tick = tick_max       |
 * 	|                       |                       |                       |                       | GPIO = GPIO_ON			|
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|                       |-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_GPIO_XX_PULSE       |                       | ST_GPIO_XX_PULSE       | tick = tick_max       |
 * 	|                       |                       |                       |                       | GPIO = GPIO_ON			|
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_GPIO_XX_ON          | EV_GPIO_XX_OFF         |                       | ST_GPIO_XX_OFF		    | GPIO = GPIO_OFF         |
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_GPIO_XX_BLINK_ON    | EV_GPIO_XX_OFF         |                       | ST_GPIO_XX_OFF         | GPIO = GPIO_OFF         |
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_GPIO_XX_ON          |                       | ST_GPIO_XX_ON          | GPIO = GPIO_ON          |
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       |                       | [tick == 0]           | ST_GPIO_XX_BLINK_OFF   | tick = tick_max       |
 * 	|                       |                       |                       |                       | GPIO = GPIO_OFF         |
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       |                       | [tick >  0]           | ST_GPIO_XX_BLINK_ON    | tick--                |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_GPIO_XX_BLINK_OFF   | EV_GPIO_XX_OFF         |                       | ST_GPIO_XX_OFF         | GPIO = GPIO_OFF         |
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       | EV_GPIO_XX_ON          |                       | ST_GPIO_XX_ON          | GPIO = GPIO_ON          |
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       |                       | [tick >  0]           | ST_GPIO_XX_BLINK_OFF   | tick--                |
 * 	|                       +-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       |                       | [tick == 0]           | ST_GPIO_XX_BLINK_ON    | tick = tick_max       |
 * 	|                       |                       |                       |                       | GPIO = GPIO_ON          |
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	| ST_GPIO_XX_PULSE       |                       | [tick == 0]           | ST_GPIO_XX_OFF         | tick = tick_max       |
 * 	|                       |                       |                       |                       | GPIO = GPIO_OFF         |
 * 	|                       |                       |                       |                       | op_GPIO(GPIO)           |
 * 	|-----------------------+-----------------------+-----------------------+-----------------------+-----------------------|
 * 	|                       |                       | [tick >  0]           | ST_GPIO_XX_PULSE       | tick--                |
 * 	------------------------+-----------------------+-----------------------+-----------------------+------------------------ */

/* Events to excite Task Actuator */
typedef enum task_GPIO_output_ev {EV_GPIO_XX_OFF,
							   EV_GPIO_XX_ON,
							   EV_GPIO_XX_NOT_BLINK,
							   EV_GPIO_XX_BLINK,
							   EV_GPIO_XX_PULSE} task_GPIO_output_ev_t;

/* States of Task Actuator */
typedef enum task_GPIO_output_st {ST_GPIO_XX_OFF,
							   ST_GPIO_XX_ON,
							   ST_GPIO_XX_BLINK_ON,
							   ST_GPIO_XX_BLINK_OFF,
							   ST_GPIO_XX_PULSE} task_GPIO_output_st_t;

/* Identifier of Task Actuator */
typedef enum task_GPIO_output_id {ID_LED_RECEIVE , ID_LED_TRANSMIT , ID_LED_ERROR , ID_BUZZER} task_GPIO_output_id_t;

typedef struct
{
	task_GPIO_output_id_t	identifier;
	GPIO_TypeDef *		gpio_port;
	uint16_t			pin;
	GPIO_PinState		GPIO_on;
	GPIO_PinState		GPIO_off;
	uint32_t			tick_blink;
	uint32_t			tick_pulse;
} task_GPIO_output_cfg_t;

typedef struct
{
	uint32_t			tick;
	task_GPIO_output_st_t	state;
	task_GPIO_output_ev_t	event;
	bool				flag;
} task_GPIO_output_dta_t;

/********************** external data declaration ****************************/
extern task_GPIO_output_dta_t task_GPIO_output_dta_list[];

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_GPIO_OUTPUT_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
