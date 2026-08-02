@echo off
setlocal

set PROJECT_DIR=%~1

set HEADER=%PROJECT_DIR%src\git.h
set TEMP=%TEMP%\git.h.tmp

set GIT_HASH=unknown
set GIT_DIRTY=false

for /f %%i in ('git rev-parse --short HEAD 2^>nul') do set GIT_HASH=%%i

for /f %%i in ('git status --porcelain 2^>nul') do (
    set GIT_DIRTY=true
    goto dirty_done
)

:dirty_done

(
echo #pragma once
echo // Auto-generated. Do not edit.
echo.
echo #define GIT_COMMIT_HASH "%GIT_HASH%"
echo #define GIT_IS_DIRTY %GIT_DIRTY%
) > "%TEMP%"

if exist "%HEADER%" (
    fc /b "%HEADER%" "%TEMP%" >nul
    if not errorlevel 1 (
        del "%TEMP%"
        echo git.h unchanged
        exit /b 0
    )
)

move /Y "%TEMP%" "%HEADER%" >nul

echo Updated git.h (%GIT_HASH%, dirty=%GIT_DIRTY%)

exit /b 0
