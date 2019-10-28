/**
  ******************************************************************************
  * @file    BSP_EEPROM.h
  * @author  Hossein Bagherzade(@realhba)
	* @email	 hossein.bagherzade@gmail.com
  * @version V1.1.1
  * @date    08-March-2018
  * @brief   Header file for BSP_EEPROM.c
  ******************************************************************************
	**/
	

#ifndef __BSP_EEPROM_H
#define __BSP_EEPROM_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "debugprobe.h"

#define EEP_DEBUG
	
#ifdef EEP_DEBUG
#include "stdio.h"	
#define EEP_LOG(...) 	aPrintOutLog( __VA_ARGS__) //Send Data on Stream...
#else
#define EEP_LOG(...)  
#endif	

#define USE_NVM_24LC16									 (1)	

#define EEPROM_7BIT_ADDRESS		 					 (uint8_t)0x50
#define	EEP_I2C_PAGESIZE								 (uint8_t)32
#define EEPROM_I2C_NUM									 I2C1
#define	EEPROM_SPEED										 100000
#define EEPROM_I2C_FLAG_TIMEOUT        	 ((uint32_t) 90)
#define EEPROM_I2C_LONG_TIMEOUT        	 ((uint32_t) (30 * EEPROM_I2C_FLAG_TIMEOUT))
                                         
#define EEP_RW_Delay										 (uint8_t)5 
	                                       
extern I2C_HandleTypeDef hi2c1;

HAL_StatusTypeDef EEPROM_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
HAL_StatusTypeDef EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);

HAL_StatusTypeDef EEPROM_SingleReadWriteTest(uint8_t eraseFlag);
HAL_StatusTypeDef EEPROM_MultipleReadWriteTest(uint8_t eraseFlag);

uint8_t BSP_EEPROM_IsConnected(void);
HAL_StatusTypeDef BSP_EEPROM_Write(uint16_t reg_address, uint8_t data_buf[], uint16_t length);
HAL_StatusTypeDef BSP_EEPROM_Read(uint16_t reg_address, uint8_t data_buf[], uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_EEPROM_H */
	
