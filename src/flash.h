#ifndef FLASH_H
#define FLASH_H
#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx_hal.h"
#include<string.h>

#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_11   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     (ADDR_FLASH_SECTOR_12 - 1)   /* End @ of user Flash area */

// Flash related function 
void Write_Flash(uint32_t address, uint32_t data);
uint32_t Read_Flash(uint32_t address);
void Write_Flash_Float(uint32_t address, float data);
float Read_Flash_Float(uint32_t address);
#ifdef __cplusplus
}
#endif
#endif // FLASH_H
