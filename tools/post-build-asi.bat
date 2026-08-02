@echo off
setlocal

set GTA2_DIR=%~1
set GTA2_EXE=%~2
set GTA2_ASI_DIR=%~3
set GTA2_ASI_FILENAME=%~4
set GTA2_ASI_PDB_FILENAME=%~5
set TARGET_PATH=%~6
set TARGET_DIR=%~7
set TARGET_NAME=%~8

echo ASI mode enabled

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

if "%GTA2_ASI_DIR%"=="" (
    echo ERROR: GTA2_ASI_DIR is not set.
    exit /b 1
)

if "%GTA2_ASI_FILENAME%"=="" (
    echo ERROR: GTA2_ASI_FILENAME is not set.
    exit /b 1
)

if not exist "%GTA2_ASI_DIR%\" (
    echo ERROR: GTA2_ASI_DIR "%GTA2_ASI_DIR%" does not exist.
    exit /b 1
)

echo Copying "%TARGET_PATH%" to "%GTA2_ASI_DIR%\%GTA2_ASI_FILENAME%"
copy /Y "%TARGET_PATH%" "%GTA2_ASI_DIR%\%GTA2_ASI_FILENAME%" >nul
if errorlevel 1 (
    echo ERROR: Failed to copy to "%GTA2_ASI_DIR%\%GTA2_ASI_FILENAME%"
    exit /b 1
)

@REM if not "%GTA2_ASI_PDB_FILENAME%"=="" (
@REM     if exist "%TARGET_DIR%\%TARGET_NAME%.pdb" (
@REM         echo Copying "%TARGET_DIR%\%TARGET_NAME%.pdb" to "%GTA2_ASI_DIR%\%GTA2_ASI_PDB_FILENAME%"
@REM         copy /Y "%TARGET_DIR%\%TARGET_NAME%.pdb" "%GTA2_ASI_DIR%\%GTA2_ASI_PDB_FILENAME%" >nul
@REM         if errorlevel 1 (
@REM             echo ERROR: Failed to copy to "%GTA2_ASI_DIR%\%GTA2_ASI_PDB_FILENAME%"
@REM             exit /b 1
@REM         )
@REM     )
@REM )

exit /b 0