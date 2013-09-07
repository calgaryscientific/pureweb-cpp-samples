@REM should typically be invoked from post build event as
@REM "$(ProjectDir)\DeployServer.bat" "$(TargetName)" "$(ProjectDir)" "$(TargetDir)"

if exist "%PUREWEB_HOME%\bin\%1" goto deploy
echo "Creating directory %PUREWEB_HOME%\bin\%1..." 
md "%PUREWEB_HOME%\bin\%1" 

:deploy
echo "Sending application files to  %PUREWEB_HOME%\bin\%1..." 
xcopy /YFDI "%PUREWEB_LIBS%\C++\VS2010\lib\*.dll" "%PUREWEB_HOME%\bin\%1" 
xcopy /YFDI %3\%1.exe "%PUREWEB_HOME%\bin\%1"

if exist "%PUREWEB_HOME%\conf" goto conf
echo "Creating directory %PUREWEB_HOME%\conf..." 
md "%PUREWEB_HOME%\conf" 

:conf
SET targetDir=###%2%###
SET targetDir=%targetDir:"###=%
SET targetDir=%targetDir:###"=%
SET targetDir=%targetDir:###=%
%PUREWEB_HOME%\bin\service-manager-cfg.exe -configFile %PUREWEB_HOME%\bin\service_config.json -action add -changeFile %targetDir%\service.json 

