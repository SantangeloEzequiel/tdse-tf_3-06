/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_HC05_attribute.h"

/********************** macros and definitions *******************************/
#define MESSAGE_UNDEFINED	'\0'
#define MAX_MESSAGES		(16)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
struct
{
	uint32_t	head;
	uint32_t	tail;
	uint32_t	count;
	char	queue[MAX_MESSAGES];
} tx_queue_task_hc05;

struct
{
	uint32_t	head;
	uint32_t	tail;
	uint32_t	count;
	char	queue[MAX_MESSAGES];
} rx_queue_task_hc05;

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_queue_messages_task_HC05(void)
{
	uint32_t i;

	tx_queue_task_hc05.head = 0;
	tx_queue_task_hc05.tail = 0;
	tx_queue_task_hc05.count = 0;

	for (i = 0; i < MAX_MESSAGES; i++)
		tx_queue_task_hc05.queue[i] = MESSAGE_UNDEFINED;

	rx_queue_task_hc05.head = 0;
	rx_queue_task_hc05.tail = 0;
	rx_queue_task_hc05.count = 0;

	for (i = 0; i < MAX_MESSAGES; i++)
		rx_queue_task_hc05.queue[i] = MESSAGE_UNDEFINED;
}

void put_tx_message_task_HC05(char message)
{
	tx_queue_task_hc05.count++;
	tx_queue_task_hc05.queue[tx_queue_task_hc05.head++] = message;

	if (MAX_MESSAGES == tx_queue_task_hc05.head)
		tx_queue_task_hc05.head = 0;
}

void put_rx_message_task_HC05(char message)
{
	rx_queue_task_hc05.count++;
	rx_queue_task_hc05.queue[rx_queue_task_hc05.head++] = message;

	if (MAX_MESSAGES == rx_queue_task_hc05.head)
		rx_queue_task_hc05.head = 0;
}

char get_tx_message_task_HC05(void)
{
	char message;

	tx_queue_task_hc05.count--;
	message = tx_queue_task_hc05.queue[tx_queue_task_hc05.tail];
	tx_queue_task_hc05.queue[tx_queue_task_hc05.tail++] = MESSAGE_UNDEFINED;

	if (MAX_MESSAGES == tx_queue_task_hc05.tail)
		tx_queue_task_hc05.tail = 0;

	return message;
}

char get_rx_message_task_HC05(void)
{
	char message;

	rx_queue_task_hc05.count--;
	message = rx_queue_task_hc05.queue[rx_queue_task_hc05.tail];
	rx_queue_task_hc05.queue[rx_queue_task_hc05.tail++] = MESSAGE_UNDEFINED;

	if (MAX_MESSAGES == rx_queue_task_hc05.tail)
		rx_queue_task_hc05.tail = 0;

	return message;
}

bool any_tx_message_task_HC05(void)
{
  return (tx_queue_task_hc05.head != tx_queue_task_hc05.tail);
}

bool any_rx_message_task_HC05(void)
{
  return (rx_queue_task_hc05.head != rx_queue_task_hc05.tail);
}


/********************** end of file ******************************************/
