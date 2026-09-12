Write-Host "===================================================" -ForegroundColor Cyan
Write-Host "  STM32H7 Car-Head Firmware Flasher (Mode=UR)" -ForegroundColor Cyan
Write-Host "===================================================" -ForegroundColor Cyan

$Loader = "CLIVEONE-W25Q64_STM32H7XX-PF10-PG6-PF8-PF9-PF7-PF6.stldr"
$Elf = "build/Debug/car-head.elf"

if (!(Test-Path $Elf)) {
    Write-Host "[INFO] Building project..." -ForegroundColor Yellow
    cmake --preset Debug
    cmake --build --preset Debug
    if ($LASTEXITCODE -ne 0) {
        Write-Host "[ERROR] Build failed!" -ForegroundColor Red
        Read-Host -Prompt "Press Enter to exit"
        exit 1
    }
}

Write-Host "[INFO] Flashing firmware via ST-Link (Connect Under Reset)..." -ForegroundColor Green
STM32_Programmer_CLI -c port=swd mode=UR -el "$Loader" -d "$Elf" -rst

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n[SUCCESS] Flashing completed successfully! MCU restarted." -ForegroundColor Green
} else {
    Write-Host "`n[ERROR] Flashing failed! Check ST-Link connection and power." -ForegroundColor Red
}

Read-Host -Prompt "Press Enter to exit"
