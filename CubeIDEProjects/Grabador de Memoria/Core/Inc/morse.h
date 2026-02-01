/*
 * morse.h
 *
 *  Created on: 31 ene 2026
 *      Author: ramaz
 */

#ifndef INC_MORSE_H_
#define INC_MORSE_H_

#define MORSE_TOTAL_SPACE_BYTES 128
#define MORSE_TOTAL_CHARACTERS 36

#define EEPROM_I2C_ADDR (0x50 << 1)

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "i2c.h"

typedef struct {
    uint8_t index;   // indice de memoria para arbol binario
    uint8_t morse;   // byte Morse codificado
    char    symbol;  // carácter asociado
} morse_entry_t;

extern const morse_entry_t morse_table[36];
extern const morse_entry_t error;

HAL_StatusTypeDef EEPROM_WriteBuffer(uint8_t memAddr, uint8_t *data, uint8_t len);

HAL_StatusTypeDef EEPROM_WriteByte(uint8_t memAddr, uint8_t data);

uint8_t EEPROM_ReadByte(uint16_t memAddr);

#endif /* INC_MORSE_H_ */
