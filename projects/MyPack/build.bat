@echo off
SETLOCAL EnableDelayedExpansion

cls

:: Destination (change as necessary)
SET "DEST=../MyPack"
SET "RIIVO=D:\Documents\Dolphin Emulator\Load\Kamek"
SET "COUNCIL=D:\EMULATION\GCNWII\DATACouncil\files\kamek"
echo %RIIVO%

:: CPP compilation settings
SET CC="../../cw/mwcceppc.exe"
SET CFLAGS=-I- -i ../../libraries -i ../MyPack -i ../../projects/CommonCodes -Cpp_exceptions off -enum int -O4,s -use_lmw_stmw on -fp hard -rostr -sdata 0 -sdata2 0 -maxerrors 1


:: CPP Sources
SET CPPFILES=
for %%f in (*.cpp) do SET "CPPFILES=%%~nf !CPPFILES!"

SET COMMONCPPFILES=
for %%f in (../CommonCodes/*.cpp) do SET "COMMONCPPFILES=%%~nf !COMMONCPPFILES!"



:: Compile CPP
SET OBJECTS=
FOR %%H IN (%CPPFILES%) DO (
    echo "Compiling %%H.cpp..."
    %CC% %CFLAGS% -c -o build/%%H.o %%H.cpp
    SET "OBJECTS=build/%%H.o !OBJECTS!"
)

SET COMMONOBJECTS=
FOR %%H IN (%COMMONCPPFILES%) DO (
    echo "Compiling %%H.cpp..."
    %CC% %CFLAGS% -c -o build/%%H.o "../CommonCodes/%%H.cpp"
    SET "COMMONOBJECTS=build/%%H.o !COMMONOBJECTS!"
)

:: Link
echo Linking...
"../../Kamek/bin/Debug/Kamek" %OBJECTS% %COMMONOBJECTS% -dynamic -externals=../../symbols.txt -versions=../../versions.txt -output-kamek=build\$KV$.bin
if %ErrorLevel% equ 0 (
	xcopy /Y build\*.bin "%RIIVO%" >NUL
	xcopy /Y build\*.bin "%COUNCIL%" >NUL
	del build\*.o >NUL
)

:end
ENDLOCAL