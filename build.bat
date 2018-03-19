@echo off

:start
echo INFO: I am running from '%cd%'

if exist bin goto changetobin

if not exist bin mkdir bin
if exist CMakeLists.txt copy CMakeLists.txt .\bin\CMakeLists.txt
if exist conanfile.txt copy conanfile.txt .\bin\conanfile.txt
if exist content\ xcopy /q /i /e /y content .\bin\content

:conan
echo INFO: Starting conan
conan install --build missing
echo INFO: Conan job finished.
goto cmake

:cmake
echo INFO: Starting cmake
cmake .. -G "Visual Studio 14 2015 Win64"
echo INFO: CMake job finished.
goto end

:changetobin
echo ERROR: I'm in the wrong directory.. moving into bin
cd bin
echo INFO: Retrying..
goto start

:end
if exist bin goto quit
cd ..
goto quit

:quit
echo.
echo.
echo INFO: Conan and CMake ran successfully. Open up your solution file (listed below) to open your project.
echo.
dir /b *.sln