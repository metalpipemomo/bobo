call git submodule update --init --recursive
call xcopy "bobo\libs\FMODAPI\api\core\lib\x64\fmod.dll" "bin\Debug-windows-x86_64\bobo\" /y
call xcopy "bobo\libs\FMODAPI\api\studio\lib\x64\fmodstudio.dll" "bin\Debug-windows-x86_64\bobo\" /y
call xcopy "bobo\libs\FMODAPI\api\core\lib\x64\fmod.dll" "bin\Release-windows-x86_64\bobo\" /y
call xcopy "bobo\libs\FMODAPI\api\studio\lib\x64\fmodstudio.dll" "bin\Release-windows-x86_64\bobo\" /y
call xcopy "bobo\libs\imgui\backends\imgui_impl_glfw.cpp" "bobo\libs\imgui\" /y
call xcopy "bobo\libs\imgui\backends\imgui_impl_glfw.h" "bobo\libs\imgui\" /y
call xcopy "bobo\libs\imgui\backends\imgui_impl_opengl3.h" "bobo\libs\imgui\" /y
call xcopy "bobo\libs\imgui\backends\imgui_impl_opengl3.cpp" "bobo\libs\imgui\" /y
call xcopy "bobo\libs\imgui\backends\imgui_impl_opengl3_loader.h" "bobo\libs\imgui\" /y
call premake\premake5.exe vs2022
PAUSE