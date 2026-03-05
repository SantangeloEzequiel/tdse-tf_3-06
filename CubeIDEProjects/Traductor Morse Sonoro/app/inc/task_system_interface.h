#ifndef INC_TASK_SYSTEM_INTERFACE_H_
#define INC_TASK_SYSTEM_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "task_system_attribute.h"
/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
extern void init_queue_event_task_system(void);
extern void init_queue_rx_task_system(void);
extern void put_event_task_system(task_system_ev_t event);
extern void put_message_rx_system(char message);
extern task_system_ev_t get_event_task_system(void);
extern char get_message_task_system(void);
extern bool any_event_task_system(void);
extern bool any_message_task_system(void);


/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_INTERFACE_H_ */

/********************** end of file ******************************************/
