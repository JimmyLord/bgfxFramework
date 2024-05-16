rem Builds the shader compiler from the bgfx folder.

rem The path for the VsDevCmd.bat file is hard-coded, not sure how to work around this.
rem This is needed for msbuild to run.
rem The final shaderc.exe is copied into the Tools folder

call "C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\Tools\VsDevCmd.bat"

pushd ..\Libraries\bgfx
..\bx\tools\bin\windows\genie --with-tools vs2022
msbuild .build\projects\vs2022\shaderc.vcxproj -p:configuration=release
copy .build\win32_vs2022\bin\shadercRelease.exe ..\..\Tools\shaderc.exe
popd
