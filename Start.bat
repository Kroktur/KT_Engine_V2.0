@echo off

IF NOT EXIST "%cd%\PrivateSetup" (
    ECHO NEED PrivateSetup FOLDER
    pause 
    GOTO END
)

:GLOBALCHOICE

ECHO Choisissez une option:
ECHO 1. Vcpkg
ECHO 2. Build
ECHO 3. Generate Doxygen
ECHO 4. End


SET "StartUpDir=%cd%\PrivateSetup"

:: Choix de base
CHOICE /C 1234 /N /M "Votre choix (1 ou 2 ou 3 ): "




IF ERRORLEVEL 4 GOTO END
IF ERRORLEVEL 3 GOTO DOXYGEN
IF ERRORLEVEL 2 GOTO BUILD
IF ERRORLEVEL 1 GOTO VCPKG

:END
EXIT /B 0

:DOXYGEN
IF NOT EXIST "%StartUpDir%\GenerateDocs.bat" (
    ECHO need GenerateDocs.bat
    pause 
    GOTO END
)
call "%StartUpDir%\GenerateDocs.bat"
GOTO GLOBALCHOICE

:BUILD
IF NOT EXIST "%StartUpDir%\Build.bat" (
    ECHO need build.bat
    pause 
    GOTO END
)
call "%StartUpDir%\Build.bat"
GOTO GLOBALCHOICE

:VCPKG
IF NOT EXIST "%StartUpDir%\VcpkgSetup.bat" (
    ECHO need VcpkgSetup.bat
    pause 
    GOTO END
)
call "%StartUpDir%\VcpkgSetup.bat"
GOTO GLOBALCHOICE
