@echo off
SET "CURRENT_DIR=%CD%"
SET "LOCAL_VCPKG_DIR=%CURRENT_DIR%\vcpkg"

:GLOBALCHOICE
ECHO 1. Installer/Utiliser vcpkg localement
ECHO 2. Utiliser un vcpkg existant ailleurs
ECHO 3. InstallAllLib
ECHO 4. UpdateAllLib
ECHO 5. Exit

CHOICE /C 12345 /N /M "Votre choix (1,2,3,4,5): "
IF ERRORLEVEL 5 GOTO END
IF ERRORLEVEL 4 GOTO UPDATEALL
IF ERRORLEVEL 3 GOTO INSTALLALL
IF ERRORLEVEL 2 GOTO USE_EXISTING
IF ERRORLEVEL 1 GOTO INSTALL_LOCAL

:END
EXIT /B 0

:INSTALL_LOCAL
ECHO.
ECHO === Installation/Utilisation locale de vcpkg ===
ECHO.

IF NOT EXIST "%LOCAL_VCPKG_DIR%" (
    ECHO Le dossier vcpkg n'existe pas. Création...
    mkdir "%LOCAL_VCPKG_DIR%"
)

IF NOT EXIST "%LOCAL_VCPKG_DIR%\vcpkg.exe" (
    ECHO Clonage de vcpkg...
    git clone https://github.com/Microsoft/vcpkg.git "%LOCAL_VCPKG_DIR%"
    CD "%LOCAL_VCPKG_DIR%"
    CALL bootstrap-vcpkg.bat
    CD "%CURRENT_DIR%"
    ECHO Installation réussie !
    ECHO Configuration de la variable d'environnement VCPKG_ROOT_DIR...
    SETX VCPKG_ROOT_DIR "%LOCAL_VCPKG_DIR%"
)

ECHO Vcpkg Set !! 

GOTO GLOBALCHOICE

:USE_EXISTING
ECHO.
ECHO === Utilisation d'un vcpkg existant ===
ECHO.
SET /P VCPKG_PATH="Entrez le chemin complet vers votre installation vcpkg: "
IF NOT EXIST "%VCPKG_PATH%\vcpkg.exe" (
    ECHO Le répertoire spécifié ne contient pas une installation valide de vcpkg.
    PAUSE
    EXIT /B 1
)
ECHO Configuration de la variable d'environnement VCPKG_ROOT_DIR...
SETX VCPKG_ROOT_DIR "%VCPKG_PATH%"
SET "LOCAL_VCPKG_DIR = %VCPKG_PATH%"
GOTO GLOBALCHOICE

:UPDATEALL

ECHO Update de Eigen...
CALL "%LOCAL_VCPKG_DIR%\vcpkg.exe" remove eigen3:x64-windows
CALL "%LOCAL_VCPKG_DIR%\vcpkg.exe" install eigen3:x64-windows
IF %ERRORLEVEL% NEQ 0 (
    ECHO Erreur lors de la mise à jour de Eigen.
    PAUSE
    EXIT /B 1
)

ECHO Update de SFML...
CALL "%LOCAL_VCPKG_DIR%\vcpkg.exe" remove sfml:x64-windows
CALL "%LOCAL_VCPKG_DIR%\vcpkg.exe" install sfml:x64-windows
IF %ERRORLEVEL% NEQ 0 (
    ECHO Erreur lors de la mise à jour de SFML.
    PAUSE
    EXIT /B 1
)

GOTO GLOBALCHOICE

:INSTALLALL

ECHO Installation de Eigen...
CALL "%LOCAL_VCPKG_DIR%\vcpkg.exe" install eigen3:x64-windows
IF %ERRORLEVEL% NEQ 0 (
    ECHO Erreur lors de l'installation de Eigen.
    PAUSE
    EXIT /B 1
)

ECHO Installation de SFML...
CALL "%LOCAL_VCPKG_DIR%\vcpkg.exe" install sfml:x64-windows
IF %ERRORLEVEL% NEQ 0 (
    ECHO Erreur lors de l'installation de SFML.
    PAUSE
    EXIT /B 1
)

GOTO GLOBALCHOICE