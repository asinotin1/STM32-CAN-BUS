#include "Bsp_W25q64.h"
#include "quadspi.h"

int8_t QSPI_W25Qxx_WriteEnable(void);
int8_t QSPI_W25Qxx_AutoPollingMemReady(void);

int8_t QSPI_W25Qxx_Init(void)
{
	uint32_t	Device_ID;

	HAL_QSPI_Abort(&hqspi);
	QSPI_W25Qxx_Reset();
	Device_ID = QSPI_W25Qxx_ReadID();

	if( Device_ID == W25Qxx_FLASH_ID)
	{
		/* Set QE Bit */
		QSPI_CommandTypeDef s_command;
		uint8_t reg[2] = {0, 0};

		// Read SR1 (0x05)
		s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
		s_command.Instruction       = 0x05; 
		s_command.AddressMode       = QSPI_ADDRESS_NONE;
		s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
		s_command.DataMode          = QSPI_DATA_1_LINE;
		s_command.DummyCycles       = 0;
		s_command.NbData            = 1;
		s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
		s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
		s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
		HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
		HAL_QSPI_Receive(&hqspi, &reg[0], HAL_QPSI_TIMEOUT_DEFAULT_VALUE);

		// Read SR2 (0x35)
		s_command.Instruction       = 0x35; 
		HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
		HAL_QSPI_Receive(&hqspi, &reg[1], HAL_QPSI_TIMEOUT_DEFAULT_VALUE);

		// If QE is not set, set it
		if ((reg[1] & 0x02) == 0)
		{
			reg[1] |= 0x02; // Set QE bit
			QSPI_W25Qxx_WriteEnable();
			
			s_command.Instruction = 0x01; // Write Status Register
			s_command.NbData = 2;
			HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
			HAL_QSPI_Transmit(&hqspi, reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
			
			QSPI_W25Qxx_AutoPollingMemReady();
		}

		return QSPI_W25Qxx_OK;
	}
	else
	{
		return W25Qxx_ERROR_INIT;
	}
}

int8_t QSPI_W25Qxx_AutoPollingMemReady(void)
{
	QSPI_CommandTypeDef     s_command;
	QSPI_AutoPollingTypeDef s_config;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.DataMode          = QSPI_DATA_1_LINE;
	s_command.DummyCycles       = 0;
	s_command.Instruction       = W25Qxx_CMD_ReadStatus_REG1;

	s_config.Match           = 0;
	s_config.MatchMode       = QSPI_MATCH_MODE_AND;
	s_config.Interval        = 0x10;
	s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
	s_config.StatusBytesSize = 1;
	s_config.Mask            = W25Qxx_Status_REG1_BUSY;

	if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_AUTOPOLLING;
	}
	return QSPI_W25Qxx_OK;

}

int8_t QSPI_W25Qxx_Reset(void)
{
	QSPI_CommandTypeDef s_command;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressMode 		 = QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.DataMode 			 = QSPI_DATA_NONE;
	s_command.DummyCycles 		 = 0;
	s_command.Instruction 		 = W25Qxx_CMD_EnableReset;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_INIT;
	}

	if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_AUTOPOLLING;
	}

	s_command.Instruction  = W25Qxx_CMD_ResetDevice;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_INIT;
	}

	if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_AUTOPOLLING;
	}
	return QSPI_W25Qxx_OK;
}

uint32_t QSPI_W25Qxx_ReadID(void)
{
	QSPI_CommandTypeDef s_command;
	uint8_t	QSPI_ReceiveBuff[3];
	uint32_t	W25Qxx_ID;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AddressMode		 = QSPI_ADDRESS_NONE;
	s_command.DataMode			 = QSPI_DATA_1_LINE;
	s_command.DummyCycles 		 = 0;
	s_command.NbData 				 = 3;
	s_command.Instruction 		 = W25Qxx_CMD_JedecID;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		return W25Qxx_ERROR_INIT;
	}

	if (HAL_QSPI_Receive(&hqspi, QSPI_ReceiveBuff, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
//		return W25Qxx_ERROR_TRANSMIT;
	}

	W25Qxx_ID = (QSPI_ReceiveBuff[0] << 16) | (QSPI_ReceiveBuff[1] << 8 ) | QSPI_ReceiveBuff[2];

	return W25Qxx_ID;
}

int8_t QSPI_W25Qxx_MemoryMappedMode(void)
{
	QSPI_CommandTypeDef      s_command;
	QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AddressMode 		 = QSPI_ADDRESS_4_LINES;
	s_command.DataMode    		 = QSPI_DATA_4_LINES;
	s_command.DummyCycles 		 = 6;
	s_command.Instruction 		 = W25Qxx_CMD_FastReadQuad_IO;

	s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
	s_mem_mapped_cfg.TimeOutPeriod     = 0;

	QSPI_W25Qxx_Reset();

	if (HAL_QSPI_MemoryMapped(&hqspi, &s_command, &s_mem_mapped_cfg) != HAL_OK)
	{
		return W25Qxx_ERROR_MemoryMapped;
	}

	return QSPI_W25Qxx_OK;
}

int8_t QSPI_W25Qxx_WriteEnable(void)
{
	QSPI_CommandTypeDef     s_command;
	QSPI_AutoPollingTypeDef s_config;

	s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
	s_command.AddressMode 			= QSPI_ADDRESS_NONE;
	s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;
	s_command.DataMode 				= QSPI_DATA_NONE;
	s_command.DummyCycles 			= 0;
	s_command.Instruction	 		= W25Qxx_CMD_WriteEnable;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_WriteEnable;	//
	}

	s_config.Match           = 0x02;
	s_config.Mask            = W25Qxx_Status_REG1_WEL;
	s_config.MatchMode       = QSPI_MATCH_MODE_AND;
	s_config.StatusBytesSize = 1;
	s_config.Interval        = 0x10;
	s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

	s_command.Instruction    = W25Qxx_CMD_ReadStatus_REG1;
	s_command.DataMode       = QSPI_DATA_1_LINE;
	s_command.NbData         = 1;

	if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_AUTOPOLLING;
	}
	return QSPI_W25Qxx_OK;
}

int8_t QSPI_W25Qxx_SectorErase(uint32_t SectorAddress)
{
	QSPI_CommandTypeDef s_command;

	s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
	s_command.AddressSize       	= QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;
	s_command.AddressMode 			= QSPI_ADDRESS_1_LINE;
	s_command.DataMode 				= QSPI_DATA_NONE;
	s_command.DummyCycles 			= 0;
	s_command.Address           	= SectorAddress;
	s_command.Instruction	 		= W25Qxx_CMD_SectorErase;

	if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_WriteEnable;
	}

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_Erase;
	}

	if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_AUTOPOLLING;
	}
	return QSPI_W25Qxx_OK;
}

int8_t QSPI_W25Qxx_BlockErase_32K (uint32_t SectorAddress)
{
	QSPI_CommandTypeDef s_command;

	s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
	s_command.AddressSize       	= QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;
	s_command.AddressMode 			= QSPI_ADDRESS_1_LINE;
	s_command.DataMode 				= QSPI_DATA_NONE;
	s_command.DummyCycles 			= 0;
	s_command.Address           	= SectorAddress;
	s_command.Instruction	 		= W25Qxx_CMD_BlockErase_32K;

	if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_WriteEnable;
	}

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_Erase;
	}

	if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_AUTOPOLLING;
	}
	return QSPI_W25Qxx_OK;
}

int8_t QSPI_W25Qxx_BlockErase_64K (uint32_t SectorAddress)
{
	QSPI_CommandTypeDef s_command;

	s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
	s_command.AddressSize       	= QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;
	s_command.AddressMode 			= QSPI_ADDRESS_1_LINE;
	s_command.DataMode 				= QSPI_DATA_NONE;
	s_command.DummyCycles 			= 0;
	s_command.Address           	= SectorAddress;
	s_command.Instruction	 		= W25Qxx_CMD_BlockErase_64K;

	if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_WriteEnable;
	}

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_Erase;
	}

	if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_AUTOPOLLING;
	}
	return QSPI_W25Qxx_OK;
}

int8_t QSPI_W25Qxx_ChipErase (void)
{
	QSPI_CommandTypeDef s_command;
	QSPI_AutoPollingTypeDef s_config;

	s_command.InstructionMode   	= QSPI_INSTRUCTION_1_LINE;
	s_command.AddressSize       	= QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode 	= QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           	= QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          	= QSPI_SIOO_INST_EVERY_CMD;
	s_command.AddressMode 			= QSPI_ADDRESS_NONE;
	s_command.DataMode 				= QSPI_DATA_NONE;
	s_command.DummyCycles 			= 0;
	s_command.Instruction	 		= W25Qxx_CMD_ChipErase;

	if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_WriteEnable;
	}

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_Erase;
	}

	s_config.Match           = 0;
	s_config.MatchMode       = QSPI_MATCH_MODE_AND;
	s_config.Interval        = 0x10;
	s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
	s_config.StatusBytesSize = 1;
	s_config.Mask            = W25Qxx_Status_REG1_BUSY;

	s_command.Instruction    = W25Qxx_CMD_ReadStatus_REG1;
	s_command.DataMode       = QSPI_DATA_1_LINE;
	s_command.NbData         = 1;


	if (HAL_QSPI_AutoPolling(&hqspi, &s_command, &s_config, W25Qxx_ChipErase_TIMEOUT_MAX) != HAL_OK)
	{
		return W25Qxx_ERROR_AUTOPOLLING;
	}
	return QSPI_W25Qxx_OK;
}

int8_t QSPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	QSPI_CommandTypeDef s_command;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AddressMode 		 = QSPI_ADDRESS_1_LINE;
	s_command.DataMode    		 = QSPI_DATA_1_LINE;
	s_command.DummyCycles 		 = 0;
	s_command.NbData      		 = NumByteToWrite;
	s_command.Address     		 = WriteAddr;
	s_command.Instruction 		 = W25Qxx_CMD_PageProgram;

	if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_WriteEnable;
	}

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_TRANSMIT;
	}

	if (HAL_QSPI_Transmit(&hqspi, pBuffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_TRANSMIT;
	}

	if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_AUTOPOLLING;
	}
	return QSPI_W25Qxx_OK;
}

int8_t QSPI_W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t Size)
{
	uint32_t end_addr, current_size, current_addr;
	uint8_t *write_data;

	current_size = W25Qxx_PageSize - (WriteAddr % W25Qxx_PageSize);

	if (current_size > Size)
	{
		current_size = Size;
	}

	current_addr = WriteAddr;
	end_addr = WriteAddr + Size;
	write_data = pBuffer;

	do
	{
		if (QSPI_W25Qxx_WriteEnable() != QSPI_W25Qxx_OK)
		{
			return W25Qxx_ERROR_WriteEnable;
		}

		else if(QSPI_W25Qxx_WritePage(write_data, current_addr, current_size) != QSPI_W25Qxx_OK)
		{
			return W25Qxx_ERROR_TRANSMIT;
		}

		else 	if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
		{
			return W25Qxx_ERROR_AUTOPOLLING;
		}

		else
		{
			current_addr += current_size;
			write_data += current_size;

			current_size = ((current_addr + W25Qxx_PageSize) > end_addr) ? (end_addr - current_addr) : W25Qxx_PageSize;
		}
	}
	while (current_addr < end_addr);

	return QSPI_W25Qxx_OK;

}

int8_t QSPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
	QSPI_CommandTypeDef s_command;

	s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	s_command.AddressMode 		 = QSPI_ADDRESS_4_LINES;
	s_command.DataMode    		 = QSPI_DATA_4_LINES;
	s_command.DummyCycles 		 = 6;
	s_command.NbData      		 = NumByteToRead;
	s_command.Address     		 = ReadAddr;
	s_command.Instruction 		 = W25Qxx_CMD_FastReadQuad_IO;

	if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_TRANSMIT;
	}

	if (HAL_QSPI_Receive(&hqspi, pBuffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return W25Qxx_ERROR_TRANSMIT;
	}

	if (QSPI_W25Qxx_AutoPollingMemReady() != QSPI_W25Qxx_OK)
	{
		return W25Qxx_ERROR_AUTOPOLLING;
	}
	return QSPI_W25Qxx_OK;
}

/*
 *
 *
 * DIDN'T TEST
 *
 */

#define W25Qxx_NumByteToTest	(10)
uint32_t W25Qxx_TestAddr  =	0;
uint8_t  W25Qxx_WriteBuffer[W25Qxx_NumByteToTest] = {1,2,3,4,5,6,7,8,9,10};
uint8_t  W25Qxx_ReadBuffer[W25Qxx_NumByteToTest];

int8_t QSPI_W25Qxx_Test(void)
{
	int32_t QSPI_Status ;
	QSPI_Status = QSPI_W25Qxx_BlockErase_32K(W25Qxx_TestAddr);

	QSPI_Status	= QSPI_W25Qxx_WriteBuffer(W25Qxx_WriteBuffer,W25Qxx_TestAddr,W25Qxx_NumByteToTest);

	QSPI_Status	= QSPI_W25Qxx_ReadBuffer(W25Qxx_ReadBuffer,W25Qxx_TestAddr,W25Qxx_NumByteToTest);

	return QSPI_Status;
}
