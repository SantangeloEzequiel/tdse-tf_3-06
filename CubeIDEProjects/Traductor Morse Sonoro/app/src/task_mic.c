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
 * @file   : task_mic.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"
#include <math.h>

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_mic_attribute.h"

/********************** macros and definitions *******************************/
#define G_TASK_SEN_CNT_INIT			0ul
#define G_TASK_SEN_TICK_CNT_INI		0ul

#define DEL_MIC_XX_MIN				0ul
#define DEL_MIC_XX_MED				25ul
#define DEL_MIC_XX_MAX				50ul

#define TIM_PRESCALER				7
#define TIM_ARR						99
/*  Freq = 10kHz = 8Mhz (Internal CLock) / (PRE+1) (ARR+1)  */

/********************** internal data declaration ****************************/
const task_mic_cfg_t task_mic_cfg_list[] = {
	{ID_MIC_INPUT, &hadc1 , &htim2 ,  MIC_PORT,  MIC_PIN, DEL_MIC_XX_MAX}
};

#define MIC_CFG_QTY	(sizeof(task_mic_cfg_list)/sizeof(task_mic_cfg_t))

task_mic_dta_t task_mic_dta_list[] = {
	{DEL_MIC_XX_MIN, ST_ADC_XX_BUSY, EV_ADC_XX_BUSY}
};

#define MIC_DTA_QTY	(sizeof(task_mic_dta_list)/sizeof(task_mic_dta_t))

//FREQUENCY ANALYSIS
#define ADC_BATCH_SIZE				100 	//MAX 100
#define FS  10000

const float target_freqs[] = {3000.0f, 4000.0f, 5000.0f};
#define NUM_TONES 3
#define THRESHOLD 5000000.0

/********************** internal functions declaration ***********************/
void task_mic_statechart(void);
float goertzel_energy(const int16_t *, int, float, float);

/********************** internal data definition *****************************/
const char *p_task_mic 		= "Task Sensor (Microphone Statechart)";
const char *p_task_mic_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_mic_cnt;
volatile uint32_t g_task_mic_tick_cnt;

/********************** external functions definition ************************/
void task_mic_init(void *parameters)
{
	uint32_t index;
	task_mic_dta_t *p_task_mic_dta;
	const task_mic_cfg_t *p_task_mic_cfg;
	task_mic_st_t state;
	task_mic_ev_t event;

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_mic_init), p_task_mic);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_mic), p_task_mic_);

	/* Init & Print out: Task execution counter */
	g_task_mic_cnt = G_TASK_SEN_CNT_INIT;
	LOGGER_INFO("   %s = %lu", GET_NAME(g_task_mic_cnt), g_task_mic_cnt);

	for (index = 0; MIC_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Data Pointer */
		p_task_mic_dta = &task_mic_dta_list[index];
		p_task_mic_cfg = &task_mic_cfg_list[index];

		/* Init & Print out: Index & Task execution FSM */
		state = ST_ADC_XX_BUSY;
		p_task_mic_dta->state = state;

		event = EV_ADC_XX_BUSY;
		p_task_mic_dta->event = event;

		HAL_TIM_Base_Start_IT(p_task_mic_cfg->timer);
		HAL_TIM_OC_Start(p_task_mic_cfg->timer, TIM_CHANNEL_2);

		//HAL_ADC_Start_IT(p_task_mic_cfg->adc);
		HAL_ADC_Start_DMA(p_task_mic_cfg->adc,
		                  (uint32_t *)p_task_mic_dta->adc_batch,
						  ADC_BATCH_SIZE);

		LOGGER_INFO(" ");
		LOGGER_INFO("   %s = %lu   %s = %lu   %s = %lu",
				    GET_NAME(index), index,
					GET_NAME(state), (uint32_t)state,
					GET_NAME(event), (uint32_t)event);
	}
}

void task_mic_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_SEN_TICK_CNT_INI < g_task_mic_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_mic_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_mic_cnt++;

		/* Run Task Sensor Statechart */
    	task_mic_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_SEN_TICK_CNT_INI < g_task_mic_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_mic_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
    }
}

void task_mic_statechart(void)
{
	uint32_t index;
	const task_mic_cfg_t *p_task_mic_cfg;
	task_mic_dta_t *p_task_mic_dta;

	for (index = 0; MIC_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Configuration & Data Pointer */
		p_task_mic_cfg = &task_mic_cfg_list[index];
		p_task_mic_dta = &task_mic_dta_list[index];

		if (p_task_mic_dta->adc_ready)
		{
			p_task_mic_dta->event =	EV_ADC_XX_READY;
		}
		else
		{
			p_task_mic_dta->event =	EV_ADC_XX_BUSY;
		}

		switch (p_task_mic_dta->state)
		{
			case ST_ADC_XX_BUSY:
				if(p_task_mic_dta->event == EV_ADC_XX_READY)
					p_task_mic_dta->state = ST_ADC_XX_READY;
				break;

			case ST_ADC_XX_READY:

				float energy = 0.0f;
				int16_t x[ADC_BATCH_SIZE];
				int32_t mean = 0;

				//Getting rid of DC
				for (int i = 0; i < ADC_BATCH_SIZE; i++)
				    mean += p_task_mic_dta->adc_batch[i];
				mean /= ADC_BATCH_SIZE;

				for (int i = 0; i < ADC_BATCH_SIZE; i++)
				    x[i] = (int16_t)(p_task_mic_dta->adc_batch[i] - mean);

				//Getting energy in the 3 to 5khz band
				for (int i = 0; i < NUM_TONES; i++)
				{
				    energy += goertzel_energy(x, ADC_BATCH_SIZE, target_freqs[i], FS);
				}

				if (energy > THRESHOLD)
				{
					HAL_GPIO_WritePin(LD2_PORT, LD2_PIN, LD2_ON);
				} else {
					HAL_GPIO_WritePin(LD2_PORT, LD2_PIN, LD2_OFF);
				}

				p_task_mic_dta->adc_ready = 0;
				p_task_mic_dta->state = ST_ADC_XX_BUSY;
				__asm("CPSIE i");
				break;


		}
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	uint32_t index;
	task_mic_dta_t *p_task_mic_dta;
	const task_mic_cfg_t *p_task_mic_cfg;

	for (index = 0; MIC_DTA_QTY > index; index++)
	{
		p_task_mic_dta = &task_mic_dta_list[index];
		p_task_mic_cfg = &task_mic_cfg_list[index];
		if(hadc->Instance == p_task_mic_cfg->adc->Instance){
			p_task_mic_dta->adc_ready = 1;
			HAL_ADC_Start_DMA(p_task_mic_cfg->adc,
			                  (uint32_t *)p_task_mic_dta->adc_batch,
							  ADC_BATCH_SIZE);
			break;
		}
	}
}


float goertzel_energy(const int16_t *x, int N, float freq, float fs)
{
    float s0 = 0.0f;
    float s1 = 0.0f;
    float s2 = 0.0f;

    float k = (N * freq) / fs;
    float w = 2.0f * M_PI * k / N;
    float coeff = 2.0f * cosf(w);

    for (int i = 0; i < N; i++)
    {
        s0 = x[i] + coeff * s1 - s2;
        s2 = s1;
        s1 = s0;
    }

    return s1*s1 + s2*s2 - coeff*s1*s2;
}
/********************** end of file ******************************************/
