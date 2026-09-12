#include "main.h"
#include "quadspi.h"
#include "Bsp_W25q64.h"

// Define loader API signatures
int Init (uint8_t Mode);
int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
int SectorErase (uint32_t EraseStartAddress, uint32_t EraseEndAddress);
int MassErase (void);

void Error_Handler(void)
{
    while(1) {}
}

/*
 * ST-Link Utility / STM32CubeProgrammer Initialization function
 * Mode: 1 (Write), 2 (Read), etc.
 */
__attribute__((used)) int Init (uint8_t Mode)
{
    // Cấu hình lại xung nhịp hệ thống (HAL_Init)
    SystemInit();
    HAL_Init();

    // Giả lập lại một phần SystemClock_Config() của hệ thống để có đủ tốc độ
    // (Bắt buộc phải set tay ở đây vì Loader chạy từ SRAM của MCU)
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);
    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 |
                                  RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);

    // Bật GPIO Port cho QSPI
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    // Khởi tạo QSPI Peripheral
    MX_QUADSPI_Init();

    // Khởi tạo và kết nối W25Q64
    if(QSPI_W25Qxx_Init() != QSPI_W25Qxx_OK) {
        return 0; // Fail
    }

    if (Mode == 1) // Write/Erase mode
    {
        // Ở chế độ nạp, ta không cần Memory-Mapped Mode
        // Nhưng nếu muốn, ta có thể reset trạng thái.
    }
    else if (Mode == 2) // Read mode
    {
        // Ở chế độ đọc/verify, Memory-Mapped rất thuận tiện
        QSPI_W25Qxx_MemoryMappedMode();
    }

    return 1; // Success
}

/*
 * Ghi dữ liệu vào QSPI Flash
 */
__attribute__((used)) int Write (uint32_t Address, uint32_t Size, uint8_t* buffer)
{
    // Bỏ offset ảo 0x90000000 do ST-Link truyền vào
    uint32_t localAddr = Address - W25Qxx_Mem_Addr;

    if (QSPI_W25Qxx_WriteBuffer(buffer, localAddr, Size) == QSPI_W25Qxx_OK)
        return 1;
    return 0;
}

/*
 * Xóa một khoảng bộ nhớ Sector theo khoảng địa chỉ
 */
__attribute__((used)) int SectorErase (uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
    uint32_t localStart = EraseStartAddress - W25Qxx_Mem_Addr;
    uint32_t localEnd = EraseEndAddress - W25Qxx_Mem_Addr;

    localStart = localStart - (localStart % 4096); // Căn lề 4KB sector

    while (localStart <= localEnd)
    {
        if (QSPI_W25Qxx_SectorErase(localStart) != QSPI_W25Qxx_OK)
            return 0;
        localStart += 4096; // Tăng 4K
    }
    return 1;
}

/*
 * Xóa toàn bộ bộ nhớ
 */
__attribute__((used)) int MassErase (void)
{
    if (QSPI_W25Qxx_ChipErase() == QSPI_W25Qxx_OK)
        return 1;
    return 0;
}
