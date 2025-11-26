@echo off
SET "CURRENT_DIR=%CD%"
SET "LOCAL_VCPKG_DIR=%CURRENT_DIR%\vcpkg"
ECHO ==== SCRIPT D'INSTALLATION VCPKG ET SFML ====
ECHO.
ECHO Choisissez une option:
ECHO 1. Installer/Utiliser vcpkg localement
ECHO 2. Utiliser un vcpkg existant ailleurs
ECHO 3. Build

:: Choix de base
CHOICE /C 123 /N /M "Votre choix (1 ou 2 ou 3 ): "

IF ERRORLEVEL 3 GOTO BUILD
IF ERRORLEVEL 2 GOTO USE_EXISTING
IF ERRORLEVEL 1 GOTO INSTALL_LOCAL

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
GOTO SFMLINSTALL

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
SET "LOCAL_VCPKG_DIR=%VCPKG_PATH%"
GOTO SFMLINSTALL

:SFMLINSTALL
ECHO Installation de SFML...
CALL "%LOCAL_VCPKG_DIR%\vcpkg.exe" install sfml:x64-windows
IF %ERRORLEVEL% NEQ 0 (
    ECHO Erreur lors de l'installation de SFML.
    PAUSE
    EXIT /B 1
)

ECHO Installation de nlohmann_json...
CALL "%LOCAL_VCPKG_DIR%\vcpkg.exe" install nlohmann-json:x64-windows
IF %ERRORLEVEL% NEQ 0 (
    ECHO Erreur lors de l'installation de nlohmann_json.
    PAUSE
    EXIT /B 1
)

:SFMLCHOICE
ECHO 1. Update SFML 
ECHO 2. Continuer
ECHO.

CHOICE /C 12 /N /M "Votre choix (1 ou 2): "
IF ERRORLEVEL 1 GOTO BUILD
IF ERRORLEVEL 2 GOTO SFMLUPDATE

:SFMLUPDATE
ECHO Suppression et réinstallation de SFML...
CALL "%LOCAL_VCPKG_DIR%\vcpkg.exe" remove sfml:x64-windows
CALL "%LOCAL_VCPKG_DIR%\vcpkg.exe" install sfml:x64-windows
IF %ERRORLEVEL% NEQ 0 (
    ECHO Erreur lors de la mise à jour de SFML.
    PAUSE
    EXIT /B 1
)
GOTO BUILD

:BUILD
ECHO Création du dossier de compilation...
IF NOT EXIST "Build" mkdir Build
CD Build
ECHO Configuration du projet avec CMake...
cmake ..
PAUSE
EXIT /B 0