pureweb-C++-samples
=====================

The following notes outline what I did to get the C++ samples built from the command line and running without the PureWeb Gravity environment. The notes below are Windows only.

1. Downloaded Tomcat server zip artifact from Bamboo and unzip to c:\temp\PureWeb\tomcat-server.

2. Downloaded C++ SDK zip artifact from Bamboo and unzip to c:\temp\PureWeb\sdk\Libs\C++

3. Clone C++ samples repository to c:\temp\PureWeb\samples\C++

4. The expanded directory structure should mimic the PureWeb Gravity setup:
    
   C:\temp\PureWeb\samples\C++\DDx
   C:\temp\PureWeb\samples\C++\Scribble
   C:\temp\PureWeb\samples\C++\ScribbleAppQt (linux only)
   C:\temp\PureWeb\sdk\Libs\C++\VS2017
   C:\temp\PureWeb\tomcat-server\conf
   C:\temp\PureWeb\tomcat-server\docs
   C:\temp\PureWeb\tomcat-server\etc
   C:\temp\PureWeb\tomcat-server\tomcat
   C:\temp\PureWeb\tomcat-server\webapp

5. Set environment variables:
   
   set PUREWEB_LIBS=c:\temp\PureWeb\sdk\Libs
   set PUREWEB_HOME=c:\temp\PureWeb
   
6. Start Visual Studio 2017, open C++\Scribble\ScribbleApp2017.sln, and build the solution.

7. Open C++\DDx\DDxService2017.sln in Visual Studio and build the solution.

8. Deploy the samples to Tomcat by copying the plugin XML files as follows:

   C:\temp\PureWeb\samples\C++\Scribble\tomcat\ScribbleCpp-plugin.xml -> PureWeb\tomcat-server\conf
   C:\temp\PureWeb\samples\C++\DDDx\tomcat\DDxCpp-plugin.xml -> PureWeb\tomcat-server\conf

7. To run Tomcat and test the samples:

   a. a PureWeb license file is required and in must be placed in c:\temp\PureWeb\tomcat-server\conf
   b. Build the HTML5 samples and deploy to Tomcat (see README for the HTML5 samples repo for instructions)
   c. cd c:\temp\PureWeb\tomcat-server\tomcat\bin
   d. catalina run
   e. Go to the apps page and run either Scribble or DDx

Note - I encountered some very odd behavior by Visual Studio after resetting the environment variables as above. Even though the new values were properly reported in Visual Studio System Info, the post-build step that calls the batch file to copy the executable and DLLs to c:\temp\PureWeb\apps was not picking up the updated value of PUREWEB_HOME but rather using the previous value pointing to my PureWeb Gravity dev environment. I am at a loss to explain this very odd behavior.

Note also, that the rakefiles used by Gravity to build the C++ samples are present, but will not function correctly because they rely on PureWeb Gravity as the ambient environment.

