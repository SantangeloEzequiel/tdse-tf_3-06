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
