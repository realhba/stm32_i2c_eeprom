/**
  ******************************************************************************
  * @file    BSP_EEPROM.c
  * @author  Hossein Bagherzade(@realhba)
	* @email	 hossein.bagherzade@gmail.com
  * @version V1.1.1
  * @date    08-March-2018
  * @brief   This file provides functions for running AT24HC128 and 24LC16 EEPROM in I2C mode
  ******************************************************************************
	**/	
	
#include "BSP_EEPROM.h"	
#include "string.h"
#include "stdlib.h"	

#define READ_WRITE_NUM 						 	(uint8_t)40
#define READ_WRITE_ADDRESS				 	(uint16_t)0
#define NVM_RANDOM_SEED				 			(uint16_t)0x1237


#if (USE_NVM_24LC16 == 1)
//==========================================================================================
static void aGetRegisterAddress(uint16_t RegAdIn, uint8_t *RegAdOut, uint8_t *DevAddress)
//==========================================================================================
{
	*DevAddress = ((0x0A << 3) | (RegAdIn / 256));
	*RegAdOut = RegAdIn - (RegAdIn / 256) * 256;
}

/**
  * @brief  check if i2c interface is functional
	* @retval value 1 in case of successful operation
  */
//====================================
uint8_t BSP_EEPROM_IsConnected(void)
//====================================
{
	for(uint8_t uCount = 0; uCount < 5; uCount++)
	{	
		if(HAL_I2C_IsDeviceReady(&hi2c1,(((0x0A << 3) | 0 ) << 1), 1, EEPROM_I2C_FLAG_TIMEOUT) == HAL_OK) return 1;
		HAL_Delay(50); 		
	}
	return 0;
}

/**
  * @brief  Writes multiple bytes to eeprom
  * @param  reg_address: eeprom register address starts from 0x0000
  * @param  data_buf: pointer to a user defined buffer for data to be copied
  * @param  length: number of bytes to be written statring from reg_address
	* @retval HAL_StatusTypeDef enum, HAL_OK in case of successful operation
  */
//==============================================================================================
HAL_StatusTypeDef BSP_EEPROM_Write(uint16_t reg_address, uint8_t data_buf[], uint16_t length)
//==============================================================================================
{
	HAL_StatusTypeDef E2PStatus = HAL_ERROR;
	uint8_t regAddressOut;
	uint8_t devAddressOut;


		for(int i=0; i<length; i++)
		{			
			aGetRegisterAddress(reg_address, &regAddressOut, &devAddressOut);
			E2PStatus = HAL_I2C_Mem_Write(&hi2c1, (devAddressOut << 1), (uint16_t)regAddressOut,
				(uint16_t) I2C_MEMADD_SIZE_8BIT, (uint8_t *)data_buf, (uint16_t) 1, (uint32_t)EEPROM_I2C_FLAG_TIMEOUT);
			reg_address++;
			data_buf++;
			HAL_Delay(EEP_RW_Delay);
		}


	
	return E2PStatus;
}

/**
  * @brief  Reads multiple bytes from eeprom
  * @param  reg_address: eeprom register address starts from 0x0000
  * @param  data_buf: pointer to a user defined buffer for data to be copied
  * @param  length: number of bytes to be restored statring from reg_address
	* @retval HAL_StatusTypeDef enum, HAL_OK in case of successful operation
  */
//============================================================================================
HAL_StatusTypeDef BSP_EEPROM_Read(uint16_t reg_address, uint8_t data_buf[], uint16_t length)
//============================================================================================
{
	HAL_StatusTypeDef E2PStatus = HAL_ERROR;
	uint8_t regAddressOut;
	uint8_t devAddressOut;
	
		for(int i=0; i<length; i++)
		{
			aGetRegisterAddress(reg_address, &regAddressOut, &devAddressOut);
			E2PStatus = HAL_I2C_Mem_Read(&hi2c1, (devAddressOut << 1), (uint16_t)regAddressOut,
				(uint16_t) I2C_MEMADD_SIZE_8BIT, (uint8_t *)data_buf, (uint16_t) 1, (uint32_t)EEPROM_I2C_FLAG_TIMEOUT);
			reg_address++;
			data_buf++;
		}

	return E2PStatus;
}
#else


/**
  * @brief  check if i2c interface is functional
	* @retval value 1 in case of successful operation
  */
//====================================
uint8_t BSP_EEPROM_IsConnected(void)
//====================================
{
	for(uint8_t uCount = 0; uCount < 5; uCount++)
	{
		if(HAL_I2C_IsDeviceReady(&hi2c1,(EEPROM_7BIT_ADDRESS<<1), 1, EEPROM_I2C_FLAG_TIMEOUT) == HAL_OK) return 1;
		HAL_Delay(50); 
	}
	return 0;
}

/**
  * @brief  Writes multiple bytes to eeprom
  * @param  reg_address: eeprom register address starts from 0x0000
  * @param  data_buf: pointer to a user defined buffer for data to be copied
  * @param  length: number of bytes to be written statring from reg_address
	* @retval HAL_StatusTypeDef enum, HAL_OK in case of successful operation
  */
//=============================================================================================
HAL_StatusTypeDef BSP_EEPROM_Write(uint16_t reg_address, uint8_t data_buf[], uint16_t length)
//=============================================================================================
{
	HAL_StatusTypeDef E2PStatus = HAL_ERROR;

		for(int i=0; i<length; i++){
			E2PStatus = HAL_I2C_Mem_Write(&hi2c1, (EEPROM_7BIT_ADDRESS<<1), (uint16_t) reg_address,
				(uint16_t) I2C_MEMADD_SIZE_16BIT, (uint8_t *)data_buf, (uint16_t) 1, (uint32_t)EEPROM_I2C_FLAG_TIMEOUT);
			reg_address++;
			data_buf++;
			HAL_Delay(EEP_RW_Delay);
		}
	
	return E2PStatus;
}

/**
  * @brief  Reads multiple bytes from eeprom
  * @param  reg_address: eeprom register address starts from 0x0000
  * @param  data_buf: pointer to a user defined buffer for data to be copied
  * @param  length: number of bytes to be restored statring from reg_address
	* @retval HAL_StatusTypeDef enum, HAL_OK in case of successful operation
  */
//==============================================================================================
HAL_StatusTypeDef BSP_EEPROM_Read(uint16_t reg_address, uint8_t data_buf[], uint16_t length)
//==============================================================================================
{
	HAL_StatusTypeDef E2PStatus = HAL_ERROR;
		for(int i=0; i<length; i++){
			E2PStatus = HAL_I2C_Mem_Read(&hi2c1, (EEPROM_7BIT_ADDRESS<<1), (uint16_t) reg_address,
				(uint16_t) I2C_MEMADD_SIZE_16BIT, (uint8_t *)data_buf, (uint16_t) 1, (uint32_t)EEPROM_I2C_FLAG_TIMEOUT);
			reg_address++;
			data_buf++;
		}
	
	return E2PStatus;
}
#endif

/**
  * @brief  reads number of bytes from eeprom 
  * @param  pBuffer: pointer to the data for read out
  * @param  WriteAddr: read address of eeprom (16bit address space)
  * @param  NumByteToRead: number of bytes for read  
	* @retval HAL_StatusTypeDef enum, HAL_OK in case of successful operation
  */
//=================================================================================================
HAL_StatusTypeDef EEPROM_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
//=================================================================================================
{
	if(NumByteToRead == 0 || pBuffer == NULL) return HAL_ERROR;
	
	return (BSP_EEPROM_Read(ReadAddr, pBuffer, NumByteToRead));
}

/**
  * @brief  writes any number of data to eeprom in PageWrite mode even if the buffer is not page aligned
  * @param  pBuffer: pointer to the data for write in
  * @param  WriteAddr: write address of eeprom (16bit address space)
  * @param  NumByteToWrite: number of bytes for write  
	* @retval HAL_StatusTypeDef enum, HAL_OK in case of successful operation
  */
//==================================================================================================
HAL_StatusTypeDef EEPROM_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
//==================================================================================================	
{
	HAL_StatusTypeDef E2PStatus = HAL_ERROR;
	
  uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
  uint16_t sEE_DataNum = 0;

  Addr = WriteAddr % EEP_I2C_PAGESIZE;
  count = EEP_I2C_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / EEP_I2C_PAGESIZE;
  NumOfSingle = NumByteToWrite % EEP_I2C_PAGESIZE;

	if(BSP_EEPROM_IsConnected() != HAL_OK){
		if(BSP_EEPROM_IsConnected() != HAL_OK) return HAL_ERROR;
	}
		
  if(Addr == 0)
	{ // WriteAddr is EEPROM_PAGESIZE aligned 
      if(NumOfPage == 0)
			{ // NumByteToWrite < EEPROM_PAGESIZE
          sEE_DataNum = NumByteToWrite;
          E2PStatus = BSP_EEPROM_Write(WriteAddr, pBuffer, sEE_DataNum);
          if (E2PStatus != HAL_OK) return E2PStatus;

      } 
			else
			{ // NumByteToWrite > EEPROM_PAGESIZE
          while (NumOfPage--)
					{
              sEE_DataNum = EEP_I2C_PAGESIZE;
              E2PStatus = BSP_EEPROM_Write(WriteAddr, pBuffer, sEE_DataNum);
              if (E2PStatus != HAL_OK) return E2PStatus;

              WriteAddr +=  EEP_I2C_PAGESIZE;
              pBuffer += EEP_I2C_PAGESIZE;
          }

          sEE_DataNum = NumOfSingle;
          E2PStatus = BSP_EEPROM_Write(WriteAddr, pBuffer, sEE_DataNum);

          if (E2PStatus != HAL_OK) return E2PStatus;
      }
  }
	else
	{ // WriteAddr is not EEPROM_PAGESIZE aligned
      if (NumOfPage == 0)
			{ // NumByteToWrite < EEPROM_PAGESIZE
          if (NumOfSingle > count)
					{ // (NumByteToWrite + WriteAddr) > EEPROM_PAGESIZE
              temp = NumOfSingle - count;
              sEE_DataNum = count;
              E2PStatus = BSP_EEPROM_Write(WriteAddr, pBuffer, sEE_DataNum);
              if (E2PStatus != HAL_OK) return E2PStatus;

              WriteAddr +=  count;
              pBuffer += count;

              sEE_DataNum = temp;
              E2PStatus = BSP_EEPROM_Write(WriteAddr, pBuffer, sEE_DataNum);
          } else
					{
              sEE_DataNum = NumByteToWrite;
              E2PStatus = BSP_EEPROM_Write(WriteAddr, pBuffer, sEE_DataNum);
          }
          if (E2PStatus != HAL_OK) return E2PStatus;
      }
			else
			{ //NumByteToWrite > EEPROM_PAGESIZE
          NumByteToWrite -= count;
          NumOfPage =  NumByteToWrite / EEP_I2C_PAGESIZE;
          NumOfSingle = NumByteToWrite % EEP_I2C_PAGESIZE;

          sEE_DataNum = count;

          E2PStatus = BSP_EEPROM_Write(WriteAddr, pBuffer, sEE_DataNum);
          if (E2PStatus != HAL_OK) return E2PStatus;

          WriteAddr +=  count;
          pBuffer += count;

          while (NumOfPage--)
					{
              sEE_DataNum = EEP_I2C_PAGESIZE;

							E2PStatus = BSP_EEPROM_Write(WriteAddr, pBuffer, sEE_DataNum);
							if (E2PStatus != HAL_OK) return E2PStatus;

              WriteAddr +=  EEP_I2C_PAGESIZE;
              pBuffer += EEP_I2C_PAGESIZE;
          }

          if (NumOfSingle != 0)
					{
              sEE_DataNum = NumOfSingle;

              E2PStatus = BSP_EEPROM_Write(WriteAddr, pBuffer, sEE_DataNum);
							if (E2PStatus != HAL_OK) return E2PStatus;
          }
      }
  }

  return HAL_OK;
}



/**
  * @brief  testing eeprom operation in single read/write mode
  * @param  eraseFlag: select if erase write area
	* @retval HAL_StatusTypeDef enum, HAL_OK in case of successful operation
  */
//=================================================================
HAL_StatusTypeDef EEPROM_SingleReadWriteTest(uint8_t eraseFlag)
//=================================================================
{
	HAL_StatusTypeDef E2PStatus = HAL_ERROR;
	uint8_t ucBuf[READ_WRITE_NUM];
	
	//Check if SPI is ready
	if(BSP_EEPROM_IsConnected() == HAL_OK)
	{
		EEP_LOG("EEPROM Is Ready\r\n");
		
		//ReadOut NVM Data
		EEP_LOG("EEPROM Data ReadOut :\r\n\r\n");
		memset(ucBuf, 0, sizeof(ucBuf));
		for(uint16_t i = READ_WRITE_ADDRESS; i < READ_WRITE_NUM + READ_WRITE_ADDRESS ; i++) {
			E2PStatus = BSP_EEPROM_Read((i - READ_WRITE_ADDRESS), &ucBuf[(i - READ_WRITE_ADDRESS)], 1);
			EEP_LOG("0x%X ", ucBuf[(i - READ_WRITE_ADDRESS)]);
		}
		EEP_LOG("\r\n\r\n");
		
		//WriteIn random data in NM
		EEP_LOG("EEPROM Data WriteIn :\r\n\r\n");
		srand(NVM_RANDOM_SEED);		
		memset(ucBuf, 0, sizeof(ucBuf));
		for(uint16_t j = READ_WRITE_ADDRESS; j < READ_WRITE_NUM + READ_WRITE_ADDRESS ; j++) 
		{
			if(eraseFlag == 0) ucBuf[(j - READ_WRITE_ADDRESS)] = rand() % 255;
			if(eraseFlag == 1) ucBuf[(j - READ_WRITE_ADDRESS)] = 0xFF;
			E2PStatus = BSP_EEPROM_Write((j - READ_WRITE_ADDRESS), &ucBuf[(j - READ_WRITE_ADDRESS)], 1);
			EEP_LOG("0x%X ", ucBuf[(j - READ_WRITE_ADDRESS)]);
		}
		EEP_LOG("\r\n\r\n");

		//ReadOut again data
		EEP_LOG("EEPROM Data ReadOut Again :\r\n\r\n");
		memset(ucBuf, 0, sizeof(ucBuf));
		for(uint16_t i = READ_WRITE_ADDRESS; i < READ_WRITE_NUM + READ_WRITE_ADDRESS ; i++) {
			E2PStatus = BSP_EEPROM_Read((i - READ_WRITE_ADDRESS), &ucBuf[(i - READ_WRITE_ADDRESS)], 1);
			EEP_LOG("0x%X ", ucBuf[(i - READ_WRITE_ADDRESS)]);
		}
		EEP_LOG("\r\n\r\n");
	}	
	
	return E2PStatus;
}

/**
  * @brief  testing eeprom operation in multiple read/write mode
  * @param  eraseFlag: select if erase write area
	* @retval HAL_StatusTypeDef enum, HAL_OK in case of successful operation
  */
//===================================================================
HAL_StatusTypeDef EEPROM_MultipleReadWriteTest(uint8_t eraseFlag)
//===================================================================
{
	HAL_StatusTypeDef E2PStatus = HAL_ERROR;
	uint8_t ucBuf[READ_WRITE_NUM];
	
	//Check if SPI is ready
	if(BSP_EEPROM_IsConnected() == HAL_OK)
	{
		EEP_LOG("EEPROM Is Ready\r\n");
		
		//ReadOut NVM Data
		EEP_LOG("EEPROM Data ReadOut :\r\n\r\n");
		memset(ucBuf, 0, sizeof(ucBuf));
		E2PStatus = EEPROM_ReadBuffer(ucBuf, READ_WRITE_ADDRESS, READ_WRITE_NUM);
		
		for(uint16_t i = 0; i < READ_WRITE_NUM; i++){
			EEP_LOG("0x%X ", ucBuf[i]);
		}
		EEP_LOG("\r\n\r\n");
		
		//WriteIn random data in NM
		EEP_LOG("EEPROM Data WriteIn :\r\n\r\n");
		srand(NVM_RANDOM_SEED);		
		memset(ucBuf, 0, sizeof(ucBuf));
		for(uint16_t j = 0; j < READ_WRITE_NUM ; j++) 
		{
			if(eraseFlag == 0) ucBuf[j] = rand() % 255;
			if(eraseFlag == 1) ucBuf[j] = 0xFF;

			EEP_LOG("0x%X ", ucBuf[j]);
		}
		EEP_LOG("\r\n\r\n");
		E2PStatus = EEPROM_WriteBuffer(ucBuf, READ_WRITE_ADDRESS, READ_WRITE_NUM);
		
		//ReadOut again
		EEP_LOG("EEPROM Data ReadOut Again :\r\n\r\n");
		memset(ucBuf, 0, sizeof(ucBuf));
		E2PStatus = EEPROM_ReadBuffer(ucBuf, READ_WRITE_ADDRESS, READ_WRITE_NUM);
		
		for(uint16_t i = 0; i < READ_WRITE_NUM; i++){
			EEP_LOG("0x%X ", ucBuf[i]);
		}
		EEP_LOG("\r\n\r\n");
	}	
	
	return E2PStatus;
}

