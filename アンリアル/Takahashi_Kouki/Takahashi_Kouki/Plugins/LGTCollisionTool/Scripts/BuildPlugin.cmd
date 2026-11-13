@echo off
setlocal
cd /d "%~dp0"

echo ========================================
echo LGTCollisionTool - UE5.7 Direct Build
echo ========================================
echo.

powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0BuildPlugin.ps1"
set "RESULT=%ERRORLEVEL%"

echo.
if "%RESULT%"=="0" (
    echo [SUCCESS] Plugin binaries were updated directly.
) else (
    echo [FAILED] Build failed. ExitCode=%RESULT%
)
echo.
pause
exit /b %RESULT%
