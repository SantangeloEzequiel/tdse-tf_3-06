#ifndef INC_TASK_SYSTEM_ATTRIBUTE_H_
#define INC_TASK_SYSTEM_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "memoria_morse.h"
/********************** macros ***********************************************/

/********************** typedef **********************************************/


/* Events to excite Task System */
typedef enum task_system_ev {EV_SYS_CONNECTION_ESTABLISHED,
							 EV_SYS_CONNECTION_LOST,
							 EV_SYS_VELOCITY1_ON,
							 EV_SYS_VELOCITY1_OFF,
							 EV_SYS_VELOCITY2_ON,
							 EV_SYS_VELOCITY2_OFF,
							 EV_SYS_VELOCITY3_ON,
							 EV_SYS_VELOCITY3_OFF,
							 EV_SYS_BTN_INPUT_ON,
							 EV_SYS_BTN_INPUT_OFF,
							 EV_SYS_MIC_INPUT_ON,
							 EV_SYS_MIC_INPUT_OFF,
								} task_system_ev_t;

/* State of Task System */
typedef enum task_system_st {ST_SYS_WAITING_CONNECTION,
							 ST_SYS_RECEIVING,
							 ST_SYS_TRANSMITTING,
							 ST_SYS_TRANSMITTING_CHAR,
							 ST_SYS_TRANSMITTING_WAIT} task_system_st_t;

typedef struct
{
	uint32_t			tick;
	task_system_st_t	state;
	task_system_ev_t	event;
	morse_entry_t 		morse_character;
	uint8_t				morse_character_tick;
	uint32_t			WPM_period;
	uint32_t			WPM_tick;
	bool 				WPM_flag;
	bool				flag;
	bool				input_signal;
	bool 				input_mic;
	bool 				input_btn;

	/* Morse RX decoder state (initialized in task_system.c). */
	morse_entry_t		rx_current_symbol;
	uint32_t			rx_high_ticks;
	uint32_t			rx_low_ticks;
	uint32_t			rx_unit_ticks;
	bool				rx_prev_signal;
	bool				rx_symbol_started;
	bool				rx_char_committed;
	uint8_t				rx_symbol_index;
	uint8_t				rx_symbol_len;

} task_system_dta_t;

/********************** external data declaration ****************************/
extern task_system_dta_t task_system_dta;
/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
