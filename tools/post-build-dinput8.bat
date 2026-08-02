@echo off
setlocal

set GTA2_DIR=%~1
set GTA2_EXE=%~2
set TARGET_PATH=%~3
set TARGET_DIR=%~4
set TARGET_NAME=%~5

echo Dinput8 proxy mode enabled

if "%GTA2_DIR%"=="" (
    echo ERROR: GTA2_DIR is not set.
    exit /b 1
)

if "%GTA2_EXE%"=="" (
    echo ERROR: GTA2_EXE is not set.
    exit /b 1
)

if not exist "%GTA2_DIR%\" (
    echo ERROR: GTA2_DIR "%GTA2_DIR%" does not exist.
    exit /b 1
)

if not exist "%GTA2_DIR%\%GTA2_EXE%" (
    echo ERROR: "%GTA2_EXE%" not found in "%GTA2_DIR%".
    exit /b 1
)

echo Copying "%TARGET_PATH%" to "%GTA2_DIR%\%TARGET_NAME%.dll"
copy /Y "%TARGET_PATH%" "%GTA2_DIR%\%TARGET_NAME%.dll" >nul
if errorlevel 1 (
    echo ERROR: Failed to copy to "%GTA2_DIR%\%TARGET_NAME%.dll"
    exit /b 1
)

@REM if exist "%TARGET_DIR%\%TARGET_NAME%.pdb" (
@REM     echo Copying "%TARGET_DIR%\%TARGET_NAME%.pdb" to "%GTA2_DIR%\%TARGET_NAME%.pdb"
@REM     copy /Y "%TARGET_DIR%\%TARGET_NAME%.pdb" "%GTA2_DIR%\%TARGET_NAME%.pdb" >nul
@REM     if errorlevel 1 (
@REM         echo ERROR: Failed to copy to "%GTA2_DIR%\%TARGET_NAME%.pdb"
@REM         exit /b 1
@REM     )
@REM )

exit /b 0
