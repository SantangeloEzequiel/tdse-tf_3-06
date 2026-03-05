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
#include "task_system_interface.h"

#include "dwt.h"


/********************** macros and definitions *******************************/
#define G_TASK_SEN_CNT_INIT			0ul
#define G_TASK_SEN_TICK_CNT_INI		0ul

#define DEL_MIC_XX_MIN				0ul
#define DEL_MIC_XX_MED				25ul
#define DEL_MIC_XX_MAX				50ul
#define MIC_DEBOUNCE_TICKS 			10

#define TIM_PRESCALER				7
#define TIM_ARR						99
/*  Freq = 10kHz = 8Mhz (Internal CLock) / (PRE+1) (ARR+1)  */

/********************** internal data declaration ****************************/
const task_mic_cfg_t task_mic_cfg_list[] = {
	{ID_MIC_INPUT, &hadc1 , &htim2 ,  MIC_PORT,  MIC_PIN, DEL_MIC_XX_MAX , EV_SYS_MIC_INPUT_OFF , EV_SYS_MIC_INPUT_ON}
};

#define MIC_CFG_QTY	(sizeof(task_mic_cfg_list)/sizeof(task_mic_cfg_t))

task_mic_dta_t task_mic_dta_list[] = {
	{DEL_MIC_XX_MIN, ST_ADC_XX_BUSY, EV_ADC_XX_BUSY , {} , 0 , false}
};

#define MIC_DTA_QTY	(sizeof(task_mic_dta_list)/sizeof(task_mic_dta_t))

//FREQUENCY ANALYSIS
#define ADC_BATCH_SIZE				50 	//MAX 100
#define FS  10000

#define NUM_TONES 3
#define THRESHOLD 75000000.0

#define Q 14
#define SCALE (1<<Q)

/********************** internal functions declaration ***********************/
void task_mic_statechart(void);
uint32_t goertzel_energy(const uint16_t *adc, int N, int16_t coeff, int mean);
uint32_t batch_energy(task_mic_dta_t *p_task_mic_dta);

/********************** internal data definition *****************************/
const char *p_task_mic 		= "Task Sensor (Microphone Statechart)";
const char *p_task_mic_ 		= "Non-Blocking & Update By Time Code";

//Precomputed for optimization
static int16_t coeffs[NUM_TONES];
const uint16_t target_freqs[] = {2000 , 3000 , 4000};

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

		//Precomputing for optimization
		for(int i=0;i<NUM_TONES;i++)
		{
		    float k = (ADC_BATCH_SIZE * target_freqs[i]) / FS;
		    float w = 2.0f * M_PI * k / ADC_BATCH_SIZE;
		    coeffs[i] = (int16_t)(2.0f * cosf(w) * SCALE);
		}
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
			p_task_mic_dta->adc_ready = 0;
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

				int32_t energy = batch_energy(p_task_mic_dta);

				/* Debounce temporal usando tick */
				if (energy > THRESHOLD)
				{
				    if (p_task_mic_dta->tick < MIC_DEBOUNCE_TICKS)
				        p_task_mic_dta->tick++;
				}
				else
				{
				    if (p_task_mic_dta->tick > 0)
				        p_task_mic_dta->tick--;
				}

				/* Confirm detection */
				if (p_task_mic_dta->tick >= MIC_DEBOUNCE_TICKS &&
				    p_task_mic_dta->mic_flag == false)
				{
				    put_event_task_system(p_task_mic_cfg->mic_detection);
				    p_task_mic_dta->mic_flag = true;
				}

				/* Confirm no detection */
				else if (p_task_mic_dta->tick == 0 &&
				         p_task_mic_dta->mic_flag == true)
				{
				    put_event_task_system(p_task_mic_cfg->mic_no_detection);
				    p_task_mic_dta->mic_flag = false;
				}
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


uint32_t goertzel_energy(const uint16_t *adc, int N, int16_t coeff, int mean)
{
    int32_t s0=0, s1=0, s2=0;

    for(int i=0;i<N;i++)
    {
        int32_t sample = adc[i] - mean;

        s0 = sample + ((coeff * s1) >> Q) - s2;

        s2 = s1;
        s1 = s0;
    }

    return (uint32_t)(
        (s1*s1) +
        (s2*s2) -
        ((coeff * s1 * s2) >> Q)
    );
}

uint32_t batch_energy(task_mic_dta_t *p_task_mic_dta)
{
    uint32_t energy = 0;
    int32_t mean = 0;

    for(int i=0;i<ADC_BATCH_SIZE;i++)
        mean += p_task_mic_dta->adc_batch[i];

    mean /= ADC_BATCH_SIZE;

    for(int i=0;i<NUM_TONES;i++)
    {
        energy += goertzel_energy(
            p_task_mic_dta->adc_batch,
            ADC_BATCH_SIZE,
            coeffs[i],
            mean
        );
    }

    return energy;
}
/********************** end of file ******************************************/
