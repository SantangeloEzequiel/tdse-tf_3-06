/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_GPIO_output_attribute.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void put_event_task_GPIO_output(task_GPIO_output_ev_t event, task_GPIO_output_id_t identifier)
{
	task_GPIO_output_dta_t *p_task_GPIO_output_dta;

	p_task_GPIO_output_dta = &task_GPIO_output_dta_list[identifier];

	p_task_GPIO_output_dta->event = event;
	p_task_GPIO_output_dta->flag = true;
}

/********************** end of file ******************************************/
