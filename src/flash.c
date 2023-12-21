#include <flash.h>


void Write_Flash(uint32_t address, uint32_t data) {
    // 解锁 Flash
    HAL_FLASH_Unlock();

    // 擦除配置
    FLASH_EraseInitTypeDef eraseInitStruct;
    uint32_t SectorError;
    eraseInitStruct.TypeErase = TYPEERASE_SECTORS;
    eraseInitStruct.VoltageRange = VOLTAGE_RANGE_3;
    eraseInitStruct.Sector = FLASH_SECTOR_11;
    eraseInitStruct.NbSectors = 1;

    // 擦除 Flash
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&eraseInitStruct, &SectorError);
    if (status != HAL_OK) {
        // 错误处理
    }

    // 写入数据
    status = HAL_FLASH_Program(TYPEPROGRAM_WORD, address, data);
    if (status != HAL_OK) {
        // 错误处理
    }

    // 锁定 Flash
    HAL_FLASH_Lock();
}

uint32_t Read_Flash(uint32_t address) {
    // 从 Flash 读取一个字的数据
    return *(uint32_t*)address;
}



void Write_Flash_Float(uint32_t address, float data) {
    
    HAL_FLASH_Unlock();
    // 由于 STM32 的 Flash 存储是按字节编程的，我们需要将 float 数据转换为 uint32_t
    uint32_t dataToWrite;
    memcpy(&dataToWrite, &data, sizeof(data));

    // 后续步骤与之前写入 uint32_t 类型数据类似，首先是擦除，然后编程

    // 解锁 Flash
    

    // 擦除配置
    FLASH_EraseInitTypeDef eraseInitStruct;
    uint32_t SectorError;
    eraseInitStruct.TypeErase = TYPEERASE_SECTORS;
    eraseInitStruct.VoltageRange = VOLTAGE_RANGE_3;
    eraseInitStruct.Sector = FLASH_SECTOR_11;
    eraseInitStruct.NbSectors = 1;

    // 擦除 Flash
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&eraseInitStruct, &SectorError);
    if (status != HAL_OK) {
       
    }

    // 写入数据
    status = HAL_FLASH_Program(TYPEPROGRAM_WORD, address, dataToWrite);
    if (status != HAL_OK) {
    }

    // 锁定 Flash
    HAL_FLASH_Lock();
}

float Read_Flash_Float(uint32_t address) {
    // 读取数据并将其从 uint32_t 转换回 float 类型
    float data;
    memcpy(&data, (uint32_t*)address, sizeof(data));
    return data;
}