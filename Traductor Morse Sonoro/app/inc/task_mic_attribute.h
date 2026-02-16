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
 * @file   : task_mic_attribute.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef TASK_INC_TASK_MIC_ATTRIBUTE_H_
#define TASK_INC_TASK_MIC_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "task_system_attribute.h"
/********************** macros ***********************************************/

/********************** typedef **********************************************/

/* Events to excite Task Mic */
typedef enum task_mic_ev {EV_ADC_XX_BUSY,
							 EV_ADC_XX_READY} task_mic_ev_t;

/* States of Task Mic */
typedef enum task_mic_st {ST_ADC_XX_BUSY,
							 ST_ADC_XX_READY} task_mic_st_t;

/* Identifier of Task Mic */
typedef enum task_mic_id {ID_MIC_INPUT} task_mic_id_t;

typedef struct
{
	task_mic_id_t		identifier;
	ADC_HandleTypeDef * adc;
	TIM_HandleTypeDef * timer;
	GPIO_TypeDef *		gpio_port;
	uint16_t			pin;
	uint32_t			tick_max;
	task_system_ev_t	mic_no_detection;
	task_system_ev_t	mic_detection;
} task_mic_cfg_t;

typedef struct
{
	uint32_t				tick;
	task_mic_st_t			state;
	task_mic_ev_t			event;
	uint16_t				adc_batch[100];
	volatile uint8_t		adc_ready;
	bool					mic_flag;
} task_mic_dta_t;

/********************** external data declaration ****************************/
extern task_mic_dta_t task_mic_dta_list[];
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_MIC_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
