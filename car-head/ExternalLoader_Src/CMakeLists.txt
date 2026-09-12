cmake_minimum_required(VERSION 3.22)
project(ExternalLoader C ASM)

# Add definitions and include paths
add_compile_definitions(
    USE_HAL_DRIVER
    STM32H743xx
)

include_directories(
    .
    ../Core/Inc
    ../Drivers/STM32H7xx_HAL_Driver/Inc
    ../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy
    ../Drivers/CMSIS/Device/ST/STM32H7xx/Include
    ../Drivers/CMSIS/Include
    ../UserLibrary
)

# Source files
set(LOADER_SOURCES
    Dev_Inf.c
    Loader_Src.c
    ../Core/Src/system_stm32h7xx.c
    ../Core/Src/quadspi.c
    ../Core/Src/stm32h7xx_hal_msp.c
    ../UserLibrary/Bsp_W25q64.c
    # HAL Drivers needed
    ../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c
    ../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c
    ../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c
    ../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c
    ../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c
    ../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_qspi.c
    ../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c
    ../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c
    ../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c
)

# Remove the hardcoded STM32H743XX_FLASH.ld from the toolchain file
string(REGEX REPLACE "-T \\\"[^\"]*STM32H743XX_FLASH.ld\\\"" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

add_executable(WeAct_H743_W25Q64.stldr ${LOADER_SOURCES})
set_target_properties(WeAct_H743_W25Q64.stldr PROPERTIES SUFFIX "")

# Set Linker Script
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker_loader.ld)
target_link_options(WeAct_H743_W25Q64.stldr PRIVATE
    -T${LINKER_SCRIPT}
    -nostartfiles
    -Wl,-Map=WeAct_H743_W25Q64.map
    -Wl,--gc-sections
    -static
)

target_compile_options(WeAct_H743_W25Q64.stldr PRIVATE
    -mcpu=cortex-m7
    -mfpu=fpv5-d16
    -mfloat-abi=hard
    -mthumb
    -Os
    -ffunction-sections
    -fdata-sections
    -fPIE
)
