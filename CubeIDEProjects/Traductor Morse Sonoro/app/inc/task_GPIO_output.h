#ifndef INC_TASK_GPIO_OUTPUT_H_
#define INC_TASK_GPIO_OUTPUT_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/
extern uint32_t g_task_GPIO_output_cnt;
extern volatile uint32_t g_task_GPIO_output_tick_cnt;

/********************** external functions declaration ***********************/
extern void task_GPIO_output_init(void *parameters);
extern void task_GPIO_output_update(void *parameters);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_GPIO_OUTPUT_H_ */

/********************** end of file ******************************************/
