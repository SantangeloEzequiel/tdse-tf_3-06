/********************** end of file ******************************************/
/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : task_system_interface.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */
/*
 * IMPLEMENTATION FOR MULTIPLE HC05s IS PENDING
 */

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
