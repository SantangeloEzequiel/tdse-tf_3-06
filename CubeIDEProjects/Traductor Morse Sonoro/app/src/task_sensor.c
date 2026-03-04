/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_sensor_attribute.h"
#include "task_system_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_SEN_CNT_INIT			0ul
#define G_TASK_SEN_TICK_CNT_INI		0ul

#define DEL_BTN_XX_MIN				0ul
#define DEL_BTN_XX_MED				25ul
#define DEL_BTN_XX_MAX				50ul

/********************** internal data declaration ****************************/
const task_sensor_cfg_t task_sensor_cfg_list[] = {
	{ID_BTN_INPUT,  BTN_INPUT_PORT,  BTN_INPUT_PIN,  BTN_INPUT_PRESSED, DEL_BTN_XX_MAX,
			EV_SYS_BTN_INPUT_ON,  EV_SYS_BTN_INPUT_OFF},
	{ID_VELOCITY1,  VELOCITY1_PORT,  VELOCITY1_PIN,  VELOCITY1_PRESSED, DEL_BTN_XX_MAX,
			EV_SYS_VELOCITY1_ON,  EV_SYS_VELOCITY1_OFF},
	{ID_VELOCITY2,  VELOCITY2_PORT,  VELOCITY2_PIN,  VELOCITY2_PRESSED, DEL_BTN_XX_MAX,
			EV_SYS_VELOCITY2_ON,  EV_SYS_VELOCITY2_OFF},
	{ID_VELOCITY3,  VELOCITY3_PORT,  VELOCITY3_PIN,  VELOCITY3_PRESSED, DEL_BTN_XX_MAX,
			EV_SYS_VELOCITY3_ON,  EV_SYS_VELOCITY3_OFF}
};

#define SENSOR_CFG_QTY	(sizeof(task_sensor_cfg_list)/sizeof(task_sensor_cfg_t))

task_sensor_dta_t task_sensor_dta_list[] = {
	{DEL_BTN_XX_MIN, ST_BTN_XX_UP, EV_BTN_XX_UP},
	{DEL_BTN_XX_MIN, ST_BTN_XX_UP, EV_BTN_XX_UP},
	{DEL_BTN_XX_MIN, ST_BTN_XX_UP, EV_BTN_XX_UP},
	{DEL_BTN_XX_MIN, ST_BTN_XX_UP, EV_BTN_XX_UP}
};

#define SENSOR_DTA_QTY	(sizeof(task_sensor_dta_list)/sizeof(task_sensor_dta_t))

/********************** internal functions declaration ***********************/
void task_sensor_statechart(void);

/********************** internal data definition *****************************/
const char *p_task_sensor 		= "Task Sensor (Sensor Statechart)";
const char *p_task_sensor_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_sensor_cnt;
volatile uint32_t g_task_sensor_tick_cnt;

/********************** external functions definition ************************/
void task_sensor_init(void *parameters)
{
	uint32_t index;
	task_sensor_dta_t *p_task_sensor_dta;
	task_sensor_st_t state;
	task_sensor_ev_t event;

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_sensor_init), p_task_sensor);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_sensor), p_task_sensor_);

	/* Init & Print out: Task execution counter */
	g_task_sensor_cnt = G_TASK_SEN_CNT_INIT;
	LOGGER_INFO("   %s = %lu", GET_NAME(g_task_sensor_cnt), g_task_sensor_cnt);

	for (index = 0; SENSOR_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Data Pointer */
		p_task_sensor_dta = &task_sensor_dta_list[index];

		/* Init & Print out: Index & Task execution FSM */
		state = ST_BTN_XX_UP;
		p_task_sensor_dta->state = state;

		event = EV_BTN_XX_UP;
		p_task_sensor_dta->event = event;

		LOGGER_INFO(" ");
		LOGGER_INFO("   %s = %lu   %s = %lu   %s = %lu",
				    GET_NAME(index), index,
					GET_NAME(state), (uint32_t)state,
					GET_NAME(event), (uint32_t)event);
	}
}

void task_sensor_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_sensor_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_sensor_cnt++;

		/* Run Task Sensor Statechart */
    	task_sensor_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_sensor_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
    }
}

void task_sensor_statechart(void)
{
	uint32_t index;
	const task_sensor_cfg_t *p_task_sensor_cfg;
	task_sensor_dta_t *p_task_sensor_dta;

	for (index = 0; SENSOR_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Configuration & Data Pointer */
		p_task_sensor_cfg = &task_sensor_cfg_list[index];
		p_task_sensor_dta = &task_sensor_dta_list[index];

		if (p_task_sensor_cfg->pressed == HAL_GPIO_ReadPin(p_task_sensor_cfg->gpio_port, p_task_sensor_cfg->pin))
		{
			p_task_sensor_dta->event =	EV_BTN_XX_DOWN;
		}
		else
		{
			p_task_sensor_dta->event =	EV_BTN_XX_UP;
		}

		switch (p_task_sensor_dta->state)
		{
			case ST_BTN_XX_UP:

				if (EV_BTN_XX_DOWN == p_task_sensor_dta->event)
				{
					p_task_sensor_dta->tick = p_task_sensor_cfg->tick_max;
					p_task_sensor_dta->state = ST_BTN_XX_FALLING;
				}

				break;

			case ST_BTN_XX_FALLING:

				p_task_sensor_dta->tick--;
				if (DEL_BTN_XX_MIN == p_task_sensor_dta->tick)
				{
					if (EV_BTN_XX_DOWN == p_task_sensor_dta->event)
					{
						put_event_task_system(p_task_sensor_cfg->signal_down);

						p_task_sensor_dta->state = ST_BTN_XX_DOWN;
					}
					else
					{
						p_task_sensor_dta->state = ST_BTN_XX_UP;
					}
				}

				break;

			case ST_BTN_XX_DOWN:

				if (EV_BTN_XX_UP == p_task_sensor_dta->event)
				{
					p_task_sensor_dta->state = ST_BTN_XX_RISING;
					p_task_sensor_dta->tick = p_task_sensor_cfg->tick_max;
				}

				break;

			case ST_BTN_XX_RISING:

				p_task_sensor_dta->tick--;
				if (DEL_BTN_XX_MIN == p_task_sensor_dta->tick)
				{
					if (EV_BTN_XX_UP == p_task_sensor_dta->event)
					{
						put_event_task_system(p_task_sensor_cfg->signal_up);

						p_task_sensor_dta->state = ST_BTN_XX_UP;
					}
					else
					{
						p_task_sensor_dta->state = ST_BTN_XX_DOWN;
					}
				}

				break;

			default:

				p_task_sensor_dta->tick  = DEL_BTN_XX_MIN;
				p_task_sensor_dta->state = ST_BTN_XX_UP;
				p_task_sensor_dta->event = EV_BTN_XX_UP;

				break;
		}
	}
}
/********************** end of file ******************************************/
