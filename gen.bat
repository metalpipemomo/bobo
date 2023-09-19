call git submodule update --init --recursive
call xcopy "bobo_engine\external\FMODAPI\api\core\lib\x64\fmod.dll" "bin\Debug-windows-x86_64\bobo_game" /y
call xcopy "bobo_engine\external\FMODAPI\api\studio\lib\x64\fmodstudio.dll" "bin\Debug-windows-x86_64\bobo_game" /y
call external\bin\premake\premake5.exe vs2022
PAUSE