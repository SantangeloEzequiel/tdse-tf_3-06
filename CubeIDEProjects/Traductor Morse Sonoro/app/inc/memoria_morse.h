#ifndef INC_MEMORIA_MORSE_H_
#define INC_MEMORIA_MORSE_H_

#define MORSE_TOTAL_SPACE_BYTES 128
#define MORSE_TOTAL_CHARACTERS 	36
#define MORSE_MAX_INDEX 		64

#define EEPROM_I2C_ADDR (0x50 << 1)

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "i2c.h"

//organización:  bits 0-2 = longitud , bits 3-7 = codigo
// 0 = punto , 1 = raya
//ordenado como arbol binario. punto = 2*x , raya = 2*x + 1

typedef enum{
	DOT,
	LINE,
	SILENCE
}morse_input;

typedef struct {
    uint8_t 		index;   // indice de memoria para arbol binario
    uint8_t 		len;
    morse_input 	morse_sequence[5];   // byte Morse codificado
    char    		ascii_symbol;  // carácter asociado
} morse_entry_t;

extern const morse_entry_t morse_table[36];
extern const morse_entry_t error;
extern const morse_entry_t starting_symbol;

HAL_StatusTypeDef EEPROM_WriteBuffer(uint8_t memAddr, uint8_t *data, uint8_t len);

HAL_StatusTypeDef EEPROM_WriteByte(uint8_t memAddr, uint8_t data);

uint8_t EEPROM_ReadByte(uint16_t memAddr);

void EEPROM_NextSymbol(morse_entry_t* currentSymbol , morse_input signal);

#endif /* INC_MORSE_H_ */
