#include "memoria_morse.h"

//organización:  bits 0-2 = longitud , bits 3-7 = codigo
// 0 = punto , 1 = raya
//ordenado como arbol binario. punto = 2*x , raya = 2*x + 1

const morse_entry_t morse_error = {0 , 0 , {SILENCE} , '?'};
const morse_entry_t starting_symbol = {1 , 0 , {SILENCE} , '#'};
const morse_entry_t morse_space = {0 , 1 , {SILENCE,SILENCE,SILENCE} , ' '};


//--------------------------------------------------WRITE OPERATIONS
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

//--------------------------------------------------READ OPERATIONS

uint8_t EEPROM_ReadByte(uint16_t memAddr)
{
    uint8_t data = 0xFF;

    HAL_I2C_Mem_Read(
        &hi2c1,               // I2C handle
        EEPROM_I2C_ADDR,      // Dirección I2C de la EEPROM
        memAddr,              // Dirección interna
        I2C_MEMADD_SIZE_8BIT, // 24C02 usa 8 bits
        &data,                // Buffer destino
        1,                    // Cantidad de bytes
        HAL_MAX_DELAY         // Timeout
    );

    return data;
}

void EEPROM_ReadSymbol(morse_entry_t* Symbol , uint16_t index){
	Symbol->index = index;
	uint8_t sequence = EEPROM_ReadByte(index);
	Symbol->len = (sequence & 0b11100000) >> 5;

	for(uint8_t i = 0 ; i < Symbol->len ; i++){
		if(sequence & (1 << i))
			Symbol->morse_sequence[i] = LINE;
		else
			Symbol->morse_sequence[i] = DOT;
	}
	Symbol->ascii_symbol = EEPROM_ReadByte(index + 1);
}

void EEPROM_NextSymbol(morse_entry_t* currentSymbol , morse_input signal){

	switch(signal){

		DOT:
		EEPROM_ReadSymbol( currentSymbol , currentSymbol->index *2 );
		break;

		LINE:
		EEPROM_ReadSymbol( currentSymbol , currentSymbol->index *2 + 1 );
		break;

		default:
		break;
	}
}

void EEPROM_SymbolToMorse( morse_entry_t* currentSymbol , char symbol){
	if(symbol != ' '){
		uint8_t symbol_index;
		if(symbol < 'A')
			symbol_index = symbol - '0' + ('Z' - 'A') + 1;
		else
			symbol_index = symbol - 'A';
		EEPROM_ReadSymbol(currentSymbol , symbol_index*2 + MORSE_TOTAL_SPACE_BYTES + 1);
	} else {
		currentSymbol = &morse_space;
	}
}

