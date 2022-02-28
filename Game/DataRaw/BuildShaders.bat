mkdir ..\Data\Shaders

rem Hard-coded to only build DX11 shaders.
shaderc -f Shaders/Basic.vert -o ../Data/Shaders/Basic.vert.bin --type v --platform windows -p vs_4_0 -i ../../Framework/Libraries/bgfx/src
shaderc -f Shaders/Basic.frag -o ../Data/Shaders/Basic.frag.bin --type f --platform windows -p ps_4_0 -i ../../Framework/Libraries/bgfx/src

pause
exit
