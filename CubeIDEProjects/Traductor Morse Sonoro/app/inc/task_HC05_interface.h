#ifndef TASK_INC_TASK_SYSTEM_INTERFACE_H_
#define TASK_INC_TASK_SYSTEM_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
extern void init_queue_messages_task_HC05(void);
extern void put_tx_message_task_HC05(char message);
extern void put_rx_message_task_HC05(char message);
extern char get_tx_message_task_HC05(void);
extern char get_rx_message_task_HC05(void);
extern bool any_tx_message_task_HC05(void);
extern bool any_rx_message_task_HC05(void);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_INTERFACE_H_ */

/********************** end of file ******************************************/
