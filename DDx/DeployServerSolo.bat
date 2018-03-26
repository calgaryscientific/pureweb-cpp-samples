@REM should typically be invoked from post build event as
@REM "$(ProjectDir)\DeployServerSolo.bat" "$(TargetName)" "$(ProjectDir)" "$(TargetDir)"
@REM should be used for deploying server for S3 upload

echo  "$(TargetDir)"

if exist "%PUREWEB_HOME%\apps\%1" goto deploy
echo "Creating directory %PUREWEB_HOME%\apps\%1..." 
md "%PUREWEB_HOME%\apps\%1" 

:deploy
echo "Sending application files to  %PUREWEB_HOME%\app\%1..."
xcopy /YFDI "%PUREWEB_LIBS%\C++\VS2015\lib\*.dll" "%PUREWEB_HOME%\apps\%1" 
xcopy /YFDI "%PUREWEB_LIBS%\C++\VS2015\lib\*.pdb" "%PUREWEB_HOME%\apps\%1"  
xcopy /YFDI %3\%1.exe "%PUREWEB_HOME%\apps\%1"
xcopy /YFDI %2\cineFrames\* "%PUREWEB_HOME%\apps\%1\cineFrames"
xcopy /YFDI %2\service.json "%PUREWEB_HOME%\apps\%1"
