/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar>.
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
 * @file   : board.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef BOARD_INC_BOARD_H_
#define BOARD_INC_BOARD_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/
#define NUCLEO_F103RC		(0)
#define NUCLEO_F303R8		(1)
#define NUCLEO_F401RE		(2)
#define NUCLEO_F446RE		(3)
#define NUCLEO_F413ZH		(4)
#define NUCLEO_F429ZI		(5)
#define NUCLEO_F439ZI		(6)
#define NUCLEO_F767ZI		(7)
#define STM32F407G_DISC1	(8)
#define STM32F429I_DISC1	(9)

#define BOARD (NUCLEO_F103RC)

/* STM32 Nucleo Boards - 32 Pins */
#if (BOARD == NUCLEO_F303R8)

#endif

/* STM32 Nucleo Boards - 64 Pins */
#if ((BOARD == NUCLEO_F103RC) || (BOARD == NUCLEO_F401RE) || (BOARD == NUCLEO_F446RE))

#define BTN_INPUT_PIN		BTN_INPUT_Pin
#define BTN_INPUT_PORT		BTN_INPUT_GPIO_Port
#define BTN_INPUT_PRESSED	GPIO_PIN_RESET
#define BTN_INPUT_HOVER		GPIO_PIN_SET

#define VELOCITY1_PIN			VELOCITY1_Pin
#define VELOCITY1_PORT			VELOCITY1_GPIO_Port
#define VELOCITY1_PRESSED			GPIO_PIN_RESET
#define VELOCITY1_HOVER			GPIO_PIN_SET

#define VELOCITY2_PIN			VELOCITY2_Pin
#define VELOCITY2_PORT			VELOCITY2_GPIO_Port
#define VELOCITY2_PRESSED		GPIO_PIN_RESET
#define VELOCITY2_HOVER			GPIO_PIN_SET

#define VELOCITY3_PIN			VELOCITY3_Pin
#define VELOCITY3_PORT			VELOCITY3_GPIO_Port
#define VELOCITY3_PRESSED			GPIO_PIN_RESET
#define VELOCITY3_HOVER			GPIO_PIN_SET

#define MIC_PIN			MIC_Pin
#define MIC_PORT		MIC_GPIO_Port

#define LED_RECEIVE_PIN			LED_RECEIVE_Pin
#define LED_RECEIVE_PORT		LED_RECEIVE_GPIO_Port
#define LED_RECEIVE_ON			GPIO_PIN_SET
#define LED_RECEIVE_OFF			GPIO_PIN_RESET

#define LED_TRANSMIT_PIN		LED_TRANSMIT_Pin
#define LED_TRANSMIT_PORT		LED_TRANSMIT_GPIO_Port
#define LED_TRANSMIT_ON			GPIO_PIN_SET
#define LED_TRANSMIT_OFF		GPIO_PIN_RESET

#define LED_ERROR_PIN		LED_ERROR_Pin
#define LED_ERROR_PORT		LED_ERROR_GPIO_Port
#define LED_ERROR_ON		GPIO_PIN_SET
#define LED_ERROR_OFF		GPIO_PIN_RESET

#define BUZZER_PIN			BUZZER_Pin
#define BUZZER_PORT			BUZZER_GPIO_Port
#define BUZZER_ON			GPIO_PIN_SET
#define BUZZER_OFF			GPIO_PIN_RESET

#define UART_RX_PORT		UART1_RX_GPIO_Port
#define UART_RX_PIN 		UART1_RX_Pin
#define UART_TX_PORT		UART1_TX_GPIO_Port
#define UART_TX_PIN			UART1_TX_Pin
#define UART_STATUS_PORT	UART1_STATUS_GPIO_Port
#define UART_STATUS_PIN		UART1_STATUS_Pin

#endif

/* STM32 Nucleo Boards - 144 Pins */
#if ((BOARD == NUCLEO_F413ZH) || (BOARD == NUCLEO_F429ZI) || (BOARD == NUCLEO_F439ZI) || (BOARD == NUCLEO_F767ZI))

//To be hardcoded

#endif

/* STM32 Discovery Kits */
#if ((BOARD == STM32F407G_DISC1) || (BOARD == STM32F429I_DISC1))

//To be hardcoded

#endif

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* BOARD_INC_BOARD_H_ */

/********************** end of file ******************************************/
