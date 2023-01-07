@echo off
set "CURRENT_PATH=%~dp0"
pushd "%CURRENT_PATH%"

call "./tools/premake5.exe" --file=project.lua vs2022

popd