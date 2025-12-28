@echo off

:GLOBALCHOICE
ECHO 1. Generate Doxygen
ECHO 2. Open html
ECHO 3. Exit

CHOICE /C 123 /N /M "Votre choix (1,2,3): "
IF ERRORLEVEL 3 GOTO END
IF ERRORLEVEL 2 GOTO OPEN
IF ERRORLEVEL 1 GOTO GENERATE


:OPEN

IF NOT EXIST "Doxygen" EXIT /B 0
CD Doxygen
IF NOT EXIST "Output" EXIT /B 0
IF EXIST "KT_Engine_Docs.html" (
    start "" "KT_Engine_Docs.html"
) ELSE (
    ECHO Le fichier KT_Engine_Docs.html n'a pas été trouvé.
    pause
    CD..
    GOTO GLOBALCHOICE
)
CD..
GOTO GLOBALCHOICE

:GENERATE
IF NOT EXIST "Doxygen" EXIT /B 0
CD Doxygen
ECHO Génération de Doxygen...
doxygen
CD..
PAUSE
GOTO GLOBALCHOICE

:END
EXIT /B 0