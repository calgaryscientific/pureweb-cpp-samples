@REM should typically be invoked from post build event as
@REM "$(ProjectDir)\DeployServer.bat" "$(ProjectName)" "$(ProjectDir)" "$(TargetDir)"

@REM %1 - $(ProjectName)
@REM %2 - $(ProjectDir)
@REM %3 - $(TargetDir)

if exist "%PUREWEB_HOME%\bin"\%1 goto deploy
md "%PUREWEB_HOME%\bin"\%1 

:deploy
xcopy /YFDI %PUREWEB_LIBS%\C++\VS2010\lib\*.dll "%PUREWEB_HOME%\bin"\%1 
xcopy /YFDI %3\DDxService.exe "%PUREWEB_HOME%\bin"\%1 

copy %2\plugin.xml "%PUREWEB_HOME%\conf\"%1-plugin.xml
