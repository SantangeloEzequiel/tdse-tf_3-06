#ifndef INC_TASK_HC05_H_
#define INC_TASK_HC05_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "task_system_attribute.h"
/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/
extern uint32_t g_task_hc05_cnt;
extern volatile uint32_t g_task_hc05_tick_cnt;

/********************** external functions declaration ***********************/
extern void task_hc05_init(void *parameters);
extern void task_hc05_update(void *parameters);
extern void task_hc05_transmit(void *parameters);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_HC05_H_ */

/********************** end of file ******************************************/
