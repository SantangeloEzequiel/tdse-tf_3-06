#ifndef INC_TASK_GPIO_OUTPUT_ATTRIBUTE_H_
#define INC_TASK_GPIO_OUTPUT_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

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
