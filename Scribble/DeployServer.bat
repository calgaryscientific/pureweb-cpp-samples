@REM should typically be invoked from post build event as
@REM "$(ProjectDir)\DeployServer.bat" "$(TargetName)" "$(ProjectDir)" "$(TargetDir)"

echo  "$(TargetDir)"

if exist "%PUREWEB_HOME%\apps\%1" goto deploy
echo "Creating directory %PUREWEB_HOME%\apps\%1..." 
md "%PUREWEB_HOME%\apps\%1" 

:deploy
echo "Sending application files to  %PUREWEB_HOME%\bin\%1..." 
xcopy /YFDI "%PUREWEB_LIBS%\C++\%4\lib\*.dll" "%PUREWEB_HOME%\apps\%1" 
xcopy /YFDI "%PUREWEB_LIBS%\C++\%4\lib\*.pdb" "%PUREWEB_HOME%\apps\%1" 
xcopy /YFDI %3\%1.exe "%PUREWEB_HOME%\apps\%1"
xcopy /YFDI %2\service.json "%PUREWEB_HOME%\apps\%1"

@REM delete copied UnitTest files
del "%PUREWEB_HOME%\apps\%1\*.UnitTests*"

@REM copy libs to $(TargetDir) to enable debugging in Visual Studio
xcopy /YFDI "%PUREWEB_LIBS%\C++\%4\lib\*.dll" "%3" 
xcopy /YFDI "%PUREWEB_LIBS%\C++\%4\lib\*.pdb" "%3" 

:conf
SET targetDir=###%2%###
SET targetDir=%targetDir:"###=%
SET targetDir=%targetDir:###"=%
SET targetDir=%targetDir:###=%

if exist %PUREWEB_HOME%\services\service-manager-cfg.exe (
	%PUREWEB_HOME%\services\service-manager-cfg.exe -configFile %PUREWEB_HOME%\services\service_config.json -action add -changeFile %targetDir%\service.json 
)
