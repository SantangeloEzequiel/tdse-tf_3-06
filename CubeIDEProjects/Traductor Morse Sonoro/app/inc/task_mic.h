#ifndef TASK_INC_TASK_MIC_H_
#define TASK_INC_TASK_MIC_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/
extern uint32_t g_task_mic_cnt;
extern volatile uint32_t g_task_mic_tick_cnt;

/********************** external functions declaration ***********************/
extern void task_mic_init(void *parameters);
extern void task_mic_update(void *parameters);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_MIC_H_ */

/********************** end of file ******************************************/
