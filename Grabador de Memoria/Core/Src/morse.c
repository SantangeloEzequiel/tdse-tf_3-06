/*
 * morse.c
 *
 *  Created on: 31 ene 2026
 *      Author: ramaz
 */


#include "morse.h"

//organización:  bits 0-2 = longitud , bits 3-7 = codigo
// 0 = punto , 1 = raya
//ordenado como arbol binario. punto = 2*x , raya = 2*x + 1

const morse_entry_t error = {0 , 0b00011111 , '?'};

const morse_entry_t morse_table[36] = {

    /* len = 1 */
    {  2, (1<<5)|0b00000, 'E' }, // .
    {  3, (1<<5)|0b00001, 'T' }, // -

    /* len = 2 */
    {  4, (2<<5)|0b00000, 'I' }, // ..
    {  6, (2<<5)|0b00001, 'N' }, // -.
    {  5, (2<<5)|0b00010, 'A' }, // .-
    {  7, (2<<5)|0b00011, 'M' }, // --

    /* len = 3 */
    {  8, (3<<5)|0b00000, 'S' }, // ...
    {  12, (3<<5)|0b00001, 'D' }, // -..
    {  10, (3<<5)|0b00010, 'R' }, // .-.
    {  14, (3<<5)|0b00011, 'G' }, // --.
    { 9, (3<<5)|0b00100, 'U' }, // ..-
    { 13, (3<<5)|0b00101, 'K' }, // -.-
    { 11, (3<<5)|0b00110, 'W' }, // .--
    { 15, (3<<5)|0b00111, 'O' }, // ---

    /* len = 4 */
    { 16, (4<<5)|0b00000, 'H' }, // ....
    { 24, (4<<5)|0b00001, 'B' }, // -...
    { 20, (4<<5)|0b00010, 'L' }, // .-..
    { 28, (4<<5)|0b00011, 'Z' }, // --..
    { 18, (4<<5)|0b00100, 'F' }, // ..-.
    { 26, (4<<5)|0b00101, 'C' }, // -.-.
    { 22, (4<<5)|0b00110, 'P' }, // .--.
    { 17, (4<<5)|0b01000, 'V' }, // ...-
    { 25, (4<<5)|0b01001, 'X' }, // -..-
    { 29, (4<<5)|0b01011, 'Q' }, // --.-
    { 27, (4<<5)|0b01101, 'Y' }, // -.--
    { 23, (4<<5)|0b01110, 'J' }, // .---

    /* len = 5 (números) */
    { 47, (5<<5)|0b01111, '1' }, // .----
    { 39, (5<<5)|0b00111, '2' }, // ..---
    { 35, (5<<5)|0b00011, '3' }, // ...--
    { 33, (5<<5)|0b00001, '4' }, // ....-
    { 32, (5<<5)|0b00000, '5' }, // .....
    { 48, (5<<5)|0b00001, '6' }, // -....
    { 56, (5<<5)|0b00011, '7' }, // --...
    { 60, (5<<5)|0b00111, '8' }, // ---..
    { 62, (5<<5)|0b01111, '9' }, // ----.
    { 63, (5<<5)|0b11111, '0' }  // -----
};

const morse_entry_t morse_table_alphabet[36] = {

	    { 0, (2<<5)|0b00010, 'A' },
	    { 1, (4<<5)|0b00001, 'B' },
	    { 2, (4<<5)|0b00101, 'C' },
	    { 3, (3<<5)|0b00001, 'D' },
	    { 4, (1<<5)|0b00000, 'E' },
	    { 5, (4<<5)|0b00100, 'F' },
	    { 6, (3<<5)|0b00011, 'G' },
	    { 7, (4<<5)|0b00000, 'H' },
	    { 8, (2<<5)|0b00000, 'I' },
	    { 9, (4<<5)|0b01110, 'J' },

	    { 10, (3<<5)|0b00101, 'K' },
	    { 11, (3<<5)|0b00010, 'L' },
	    { 12, (2<<5)|0b00011, 'M' },  // ROOT
	    { 13, (2<<5)|0b00001, 'N' },
	    { 14, (3<<5)|0b00111, 'O' },
	    { 15, (4<<5)|0b00110, 'P' },
	    { 16, (4<<5)|0b01011, 'Q' },
	    { 17, (3<<5)|0b00010, 'R' },
	    { 18, (3<<5)|0b00000, 'S' },
	    { 19, (1<<5)|0b00001, 'T' },

	    { 20, (3<<5)|0b00100, 'U' },
	    { 21, (4<<5)|0b01000, 'V' },
	    { 22, (3<<5)|0b00110, 'W' },
	    { 23, (4<<5)|0b01001, 'X' },
	    { 24, (4<<5)|0b01101, 'Y' },
	    { 25, (4<<5)|0b00011, 'Z' },

	    { 26, (5<<5)|0b11111, '0' },
	    { 27, (5<<5)|0b01111, '1' },
	    { 28, (5<<5)|0b00111, '2' },
	    { 29, (5<<5)|0b00011, '3' },
	    { 30, (5<<5)|0b00001, '4' },
	    { 31, (5<<5)|0b00000, '5' },
	    { 32, (5<<5)|0b00001, '6' },
	    { 33, (5<<5)|0b00011, '7' },
	    { 34, (5<<5)|0b00111, '8' },
	    { 35, (5<<5)|0b01111, '9' },
};

HAL_StatusTypeDef EEPROM_WaitForWrite(void)
{
    while (HAL_I2C_IsDeviceReady(&hi2c1,
                                 EEPROM_I2C_ADDR,
                                 1,
                                 HAL_MAX_DELAY) != HAL_OK)
    {
        // esperar hasta que la EEPROM responda
    }
    return HAL_OK;
}

HAL_StatusTypeDef EEPROM_WriteByte(uint8_t memAddr, uint8_t data)
{
    uint8_t buffer[2] = { memAddr, data };

    HAL_StatusTypeDef status =
        HAL_I2C_Master_Transmit(&hi2c1,
                                 EEPROM_I2C_ADDR,
                                 buffer,
                                 2,
                                 HAL_MAX_DELAY);

    if (status != HAL_OK) return status;

    return EEPROM_WaitForWrite();
}

uint8_t EEPROM_ReadByte(uint16_t memAddr)
{
    uint8_t data = 0xFF;

    HAL_StatusTypeDef status =
        HAL_I2C_Mem_Read(&hi2c1,
                         EEPROM_I2C_ADDR,
                         memAddr,
                         I2C_MEMADD_SIZE_8BIT,
                         &data,
                         1,
                         100);

    if(status == HAL_OK)
        return data;

    if(status == HAL_TIMEOUT)
        return 0xEE;

    if(status == HAL_ERROR)
        return 0xDD;

    return 0xCC;
}

HAL_StatusTypeDef EEPROM_WriteBuffer(uint8_t memAddr,
                                     uint8_t *data,
                                     uint8_t len)
{
    if (len > 8) return HAL_ERROR;

    uint8_t buffer[9];
    buffer[0] = memAddr;

    for (uint8_t i = 0; i < len; i++)
        buffer[i + 1] = data[i];

    HAL_StatusTypeDef status =
        HAL_I2C_Master_Transmit(&hi2c1,
                                 EEPROM_I2C_ADDR,
                                 buffer,
                                 len + 1,
                                 HAL_MAX_DELAY);

    if (status != HAL_OK) return status;

    return EEPROM_WaitForWrite();
}
