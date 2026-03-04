/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_system_attribute.h"

/********************** macros and definitions *******************************/
#define MESSAGE_UNDEFINED 	('\0')
#define EVENT_UNDEFINED		(255)
#define MAX_MESSAGES		(255)
#define MAX_EVENTS			(16)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
struct
{
	uint32_t	head;
	uint32_t	tail;
	uint32_t	count;
	task_system_ev_t	queue[MAX_EVENTS];
} queue_task_system;

struct
{
	uint32_t	head;
	uint32_t	tail;
	uint32_t	count;
	char	queue[MAX_MESSAGES];
} queue_rx_system;

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_queue_event_task_system(void)
{
	uint32_t i;

	queue_task_system.head = 0;
	queue_task_system.tail = 0;
	queue_task_system.count = 0;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_task_system.queue[i] = EVENT_UNDEFINED;
}

void init_queue_rx_task_system(void)
{
	uint32_t i;

	queue_rx_system.head = 0;
	queue_rx_system.tail = 0;
	queue_rx_system.count = 0;

	for (i = 0; i < MAX_MESSAGES; i++)
		queue_rx_system.queue[i] = MESSAGE_UNDEFINED;
}

void put_event_task_system(task_system_ev_t event)
{

	queue_task_system.count++;
	queue_task_system.queue[queue_task_system.head++] = event;

	if (MAX_EVENTS == queue_task_system.head)
		queue_task_system.head = 0;
}

void put_message_rx_system(char message)
{
	queue_rx_system.count++;
	queue_rx_system.queue[queue_rx_system.head++] = message;

	if (MAX_MESSAGES == queue_rx_system.head)
		queue_rx_system.head = 0;
}

task_system_ev_t get_event_task_system(void)
{
	task_system_ev_t event;

	queue_task_system.count--;
	event = queue_task_system.queue[queue_task_system.tail];
	queue_task_system.queue[queue_task_system.tail++] = EVENT_UNDEFINED;

	if (MAX_EVENTS == queue_task_system.tail)
		queue_task_system.tail = 0;

	return event;
}

char get_message_task_system(void)
{
	char message;

	queue_rx_system.count--;
	message = queue_rx_system.queue[queue_rx_system.tail];
	queue_rx_system.queue[queue_rx_system.tail++] = MESSAGE_UNDEFINED;

	if (MAX_MESSAGES == queue_rx_system.tail)
		queue_task_system.tail = 0;

	return message;
}

bool any_event_task_system(void)
{
  return (queue_task_system.head != queue_task_system.tail);
}

bool any_message_task_system(void)
{
  return (queue_rx_system.head != queue_rx_system.tail);
}



/********************** end of file ******************************************/
