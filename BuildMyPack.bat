@echo off
SETLOCAL EnableDelayedExpansion

cls

:: Destination (change as necessary)
SET "SOURCE=MyPack"
SET "RIIVO=D:\Documents\Dolphin Emulator\Load\Kamek"
SET "COUNCIL=D:\EMULATION\GCNWII\DATACouncil\files\kamek"
echo %RIIVO%

:: CPP compilation settings
SET CC="cw/mwcceppc.exe"
SET CFLAGS=-I- -i libraries -i projects/%SOURCE% -i projects/CommonCodes -Cpp_exceptions off -enum int -O4,s -use_lmw_stmw on -fp hard -rostr -sdata 0 -sdata2 0 -maxerrors 1 -d NANDSZSLOADER -d BOOTMENUS -d GHOSTLOADER

:: CPP Sources
SET CPPFILES=
for %%f in (projects/%SOURCE%/*.cpp) do SET "CPPFILES=%%~nf !CPPFILES!"

SET COMMONCPPFILES=
for %%f in (projects/CommonCodes/*.cpp) do SET "COMMONCPPFILES=%%~nf !COMMONCPPFILES!"



:: Compile CPP
SET OBJECTS=
FOR %%H IN (%CPPFILES%) DO (
    echo "Compiling %%H.cpp..."
    %CC% %CFLAGS% %DEFINE% -c -o "projects/%SOURCE%/build/%%H.o" "projects/%SOURCE%/%%H.cpp"
    SET "OBJECTS=projects/%SOURCE%/build/%%H.o !OBJECTS!"
)

SET COMMONOBJECTS=
FOR %%H IN (%COMMONCPPFILES%) DO (
    echo "Compiling %%H.cpp..."
    %CC% %CFLAGS% %DEFINE% -c -o "projects/%SOURCE%/build/%%H.o" "projects/CommonCodes/%%H.cpp"
    SET "COMMONOBJECTS=projects/%SOURCE%/build/%%H.o !COMMONOBJECTS!"
)

:: Link
echo Linking...
"Kamek/bin/Debug/Kamek" %OBJECTS% %COMMONOBJECTS% -dynamic -externals=symbols.txt -versions=versions.txt -output-kamek=projects\%SOURCE%\build\$KV$.bin
echo Done
if %ErrorLevel% equ 0 (
	xcopy /Y projects\%SOURCE%\build\*.bin "%RIIVO%" >NUL
    xcopy /Y projects\%SOURCE%\build\*.bin "%COUNCIL%" >NUL
	del projects\%SOURCE%\build\*.o >NUL
)

:end
ENDLOCAL