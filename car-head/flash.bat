@echo off
setlocal enabledelayedexpansion

echo ===================================================
echo   STM32H7 Car-Head Firmware Flasher (Mode=UR)
echo ===================================================

set LOADER=CLIVEONE-W25Q64_STM32H7XX-PF10-PG6-PF8-PF9-PF7-PF6.stldr
set ELF=build\Debug\car-head.elf

if not exist "%ELF%" (
    echo [INFO] Firmware binary not found. Building project...
    cmake --preset Debug
    cmake --build --preset Debug
    if errorlevel 1 (
        echo [ERROR] Build failed!
        pause
        exit /b 1
    )
)

echo [INFO] Flashing firmware to MCU via ST-Link (Connect Under Reset)...
STM32_Programmer_CLI -c port=swd mode=UR -el "%LOADER%" -d "%ELF%" -rst

if errorlevel 1 (
    echo.
    echo [ERROR] Flashing failed! Please check ST-Link connection.
) else (
    echo.
    echo [SUCCESS] Flash completed successfully! MCU restarted.
)

pause
