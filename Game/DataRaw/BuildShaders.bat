@setlocal EnableDelayedExpansion

@echo off

@mkdir ..\Data\Shaders
@mkdir ..\Data\Shaders\dx9
@mkdir ..\Data\Shaders\dx11
rem @mkdir ..\Data\Shaders\pssl
rem @mkdir ..\Data\Shaders\metal
rem @mkdir ..\Data\Shaders\nvn
@mkdir ..\Data\Shaders\glsl
rem @mkdir ..\Data\Shaders\essl
@mkdir ..\Data\Shaders\spirv

@call; rem clear the errorlevel

@FOR %%G in ("Shaders\*.vert") DO (
    shaderc -f %%G -o ../Data/Shaders/dx9/%%~nxG.bin   --type v --platform windows -i ../../Framework/Libraries/bgfx/src -p vs_3_0
    IF !ERRORLEVEL! NEQ 0 (goto end) ELSE (echo Built dx9 %%G)
    shaderc -f %%G -o ../Data/Shaders/dx11/%%~nxG.bin  --type v --platform windows -i ../../Framework/Libraries/bgfx/src -p vs_4_0
    IF !ERRORLEVEL! NEQ 0 (goto end) ELSE (echo Built dx11 %%G)
    shaderc -f %%G -o ../Data/Shaders/glsl/%%~nxG.bin  --type v --platform windows -i ../../Framework/Libraries/bgfx/src -p 440
    IF !ERRORLEVEL! NEQ 0 (goto end) ELSE (echo Built glsl %%G)
    shaderc -f %%G -o ../Data/Shaders/spirv/%%~nxG.bin --type v --platform windows -i ../../Framework/Libraries/bgfx/src -p spirv
    IF !ERRORLEVEL! NEQ 0 (goto end) ELSE (echo Built spirv %%G)
)

IF !ERRORLEVEL! NEQ 0 (goto end)

@FOR %%G in ("Shaders\*.frag") DO (
    shaderc -f %%G -o ../Data/Shaders/dx9/%%~nxG.bin   --type f --platform windows -i ../../Framework/Libraries/bgfx/src -p ps_3_0
    IF !ERRORLEVEL! NEQ 0 (goto end) ELSE (echo Built dx9 %%G)
    shaderc -f %%G -o ../Data/Shaders/dx11/%%~nxG.bin  --type f --platform windows -i ../../Framework/Libraries/bgfx/src -p ps_4_0
    IF !ERRORLEVEL! NEQ 0 (goto end) ELSE (echo Built dx11 %%G)
    shaderc -f %%G -o ../Data/Shaders/glsl/%%~nxG.bin  --type f --platform windows -i ../../Framework/Libraries/bgfx/src -p 440
    IF !ERRORLEVEL! NEQ 0 (goto end) ELSE (echo Built glsl %%G)
    shaderc -f %%G -o ../Data/Shaders/spirv/%%~nxG.bin --type f --platform windows -i ../../Framework/Libraries/bgfx/src -p spirv
    IF !ERRORLEVEL! NEQ 0 (goto end) ELSE (echo Built spirv %%G)
)

echo No errors.

:end
pause
exit
