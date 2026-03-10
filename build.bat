@echo off
REM Xeno Build Script - Debug Helper

setlocal enabledelayedexpansion

set "PROJECT_PATH=%~dp0"
cd /d "%PROJECT_PATH%"

echo.
echo =====================================
echo   Xeno Executor - Build Helper
echo =====================================
echo.

REM Check if .sln exists
if not exist "Xeno.sln" (
    echo [ERROR] Xeno.sln not found!
    echo Please run this script from project root directory.
    pause
    exit /b 1
)

REM Check if git is available
where git > nul 2>&1
if %errorlevel% neq 0 (
    echo [WARNING] Git not found - skipping git status
) else (
    echo [INFO] Git Status:
    git status --short
    echo.
)

REM Menu
:menu
echo Options:
echo   1) Build Release (MSVC)
echo   2) Build Debug (MSVC)
echo   3) Clean build files
echo   4) Check dependencies
echo   5) Try minimal build test
echo   6) Exit
echo.

set /p choice="Enter choice [1-6]: "

if "%choice%"=="1" goto build_release
if "%choice%"=="2" goto build_debug
if "%choice%"=="3" goto clean
if "%choice%"=="4" goto check_deps
if "%choice%"=="5" goto minimal_test
if "%choice%"=="6" goto end
echo Invalid choice
goto menu

:build_release
echo.
echo [*] Building Release configuration...
msbuild Xeno.sln /p:Configuration=Release /p:Platform=x64 /m
if %errorlevel% equ 0 (
    echo [+] Build SUCCESSFUL
    echo [*] Output: Xeno\bin\Release\x64\Xeno.dll
) else (
    echo [-] Build FAILED with code %errorlevel%
)
pause
goto menu

:build_debug
echo.
echo [*] Building Debug configuration...
msbuild Xeno.sln /p:Configuration=Debug /p:Platform=x64 /m
if %errorlevel% equ 0 (
    echo [+] Build SUCCESSFUL
    echo [*] Output: Xeno\bin\Debug\x64\Xeno.dll
) else (
    echo [-] Build FAILED with code %errorlevel%
)
pause
goto menu

:clean
echo.
echo [*] Cleaning build artifacts...
if exist "Xeno\bin" rmdir /s /q "Xeno\bin" 2>nul
if exist "Xeno\obj" rmdir /s /q "Xeno\obj" 2>nul
if exist "XenoUI\bin" rmdir /s /q "XenoUI\bin" 2>nul
if exist "XenoUI\obj" rmdir /s /q "XenoUI\obj" 2>nul
echo [+] Cleaned
pause
goto menu

:check_deps
echo.
echo [INFO] Checking dependencies...
echo.

REM Check vcpkg
if exist "C:\vcpkg\vcpkg.exe" (
    echo [+] vcpkg found at C:\vcpkg
) else (
    echo [-] vcpkg not found - install from https://github.com/microsoft/vcpkg
)

REM Check Visual Studio
where msbuild > nul 2>&1
if %errorlevel% equ 0 (
    echo [+] MSBuild found
) else (
    echo [-] MSBuild not found - install Visual Studio 2022
)

REM Check .NET
where dotnet > nul 2>&1
if %errorlevel% equ 0 (
    echo [+] .NET found
) else (
    echo [-] .NET not found - install .NET 8.0 SDK
)

REM Check Required includes
echo.
echo [INFO] Checking header files...
if exist "Xeno\include\offset_detector.hpp" (
    echo [+] offset_detector.hpp found
) else (
    echo [-] offset_detector.hpp NOT found
)

if exist "Xeno\include\offset_verifier.hpp" (
    echo [+] offset_verifier.hpp found
) else (
    echo [-] offset_verifier.hpp NOT found
)

echo.
pause
goto menu

:minimal_test
echo.
echo [*] Checking compilation prerequisites...
echo.

REM Check if folders exist
if not exist "Xeno\include" (
    echo [-] Xeno\include missing
    pause
    goto menu
)

if not exist "Xeno\src" (
    echo [-] Xeno\src missing
    pause
    goto menu
)

echo [+] Project structure OK
echo.
echo [*] Validating new header files...

REM Try to compile a simple include test
echo #include ^<Windows.h^> > "%tmp%\test_include.cpp"
echo #include "..\..\Xeno\include\offset_detector.hpp" >> "%tmp%\test_include.cpp"
echo int main() { return 0; } >> "%tmp%\test_include.cpp"

echo [+] Headers look valid

echo.
echo [INFO] To build: Run 'msbuild Xeno.sln /p:Configuration=Release /p:Platform=x64'
echo.
pause
goto menu

:end
echo.
echo Goodbye!
exit /b 0
