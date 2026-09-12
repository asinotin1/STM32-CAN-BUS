param (
    [string]$ToolchainFile = "../cmake/gcc-arm-none-eabi.cmake"
)

$BuildDir = "build"

if (!(Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

Set-Location $BuildDir

Write-Host "Configuring CMake for External Loader..." -ForegroundColor Cyan
cmake -DCMAKE_TOOLCHAIN_FILE="$ToolchainFile" ..

Write-Host "Building External Loader..." -ForegroundColor Cyan
cmake --build .

if (Test-Path "WeAct_H743_W25Q64.stldr") {
    Write-Host "=========================================" -ForegroundColor Green
    Write-Host "Success! The External Loader is ready:" -ForegroundColor Green
    Write-Host "$PWD\WeAct_H743_W25Q64.stldr" -ForegroundColor Yellow
    Write-Host "Copying to root folder..." -ForegroundColor Cyan
    Copy-Item "WeAct_H743_W25Q64.stldr" "..\..\WeAct_H743_W25Q64.stldr" -Force
    Write-Host "=========================================" -ForegroundColor Green
} else {
    Write-Host "Build failed." -ForegroundColor Red
}

Set-Location ..
