@echo off
:: PuttyAlt SmartScreen Unblock Script
:: Run this after downloading puttyalt.exe to remove the SmartScreen warning.
:: Right-click -> Run as Administrator (if needed)

echo ============================================
echo   PuttyAlt SmartScreen Unblock Utility
echo ============================================
echo.

set "EXEPATH=%~dp0..\puttyalt.exe"
if "%~1" neq "" set "EXEPATH=%~1"

if not exist "%EXEPATH%" (
    echo ERROR: File not found: %EXEPATH%
    echo Usage: unblock-exe.bat [path\to\puttyalt.exe]
    pause
    exit /b 1
)

echo Removing SmartScreen block from: %EXEPATH%
echo.

:: Method 1: PowerShell Unblock-File
powershell -NoProfile -Command "Unblock-File -Path '%EXEPATH%'" 2>nul
if %errorlevel% equ 0 (
    echo [OK] PowerShell Unblock-File succeeded
) else (
    echo [SKIP] PowerShell method failed, trying alternative...
)

:: Method 2: Delete Zone.Identifier ADS directly
echo:>"%EXEPATH%:Zone.Identifier" 2>nul
del "%EXEPATH%:Zone.Identifier" 2>nul
echo [OK] Zone.Identifier ADS removed

:: Verify
powershell -NoProfile -Command "if (Get-Item '%EXEPATH%' -Stream Zone.Identifier -ErrorAction SilentlyContinue) { exit 1 } else { exit 0 }" 2>nul
if %errorlevel% equ 0 (
    echo.
    echo SUCCESS: SmartScreen block has been removed.
    echo You can now run puttyalt.exe without warnings.
) else (
    echo.
    echo WARNING: Could not fully remove the block.
    echo Try right-clicking puttyalt.exe -^> Properties -^> Unblock
)

echo.
pause
