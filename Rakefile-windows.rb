dir = File.dirname(__FILE__)
VS2010_DDX_SLN = "#{dir}\\DDx\\DDxService2010.sln"
VS2010_SCRIBBLE_SLN = "#{dir}\\Scribble\\ScribbleApp2010.sln"

# Clean files left behind by Visual Studio
def clean_debris 
    objdirs = File.join("**/", "obj")
    userfiles = File.join("**/", "*.vcxproj.user")

    delete_list = FileList.new(objdirs, userfiles)
    delete_list.each do |file|
        puts "Removing #{file}"
        FileUtils.rm_rf("#{file}")
    end
end

#### Task Definintions

desc "Setup the C++ environment"
task :setup do	
    fail("CSI_LIB environment variable is not set. .Net build requires it") if !ENV["CSI_LIB"]
	
	logfiles = File.join("#{BUILD_DIR}/logs", "*samples*.log")
    delete_list = FileList.new(logfiles)
    delete_list.each do |file|
        puts "Removing #{file}"
        FileUtils.rm_rf("#{file}")
    end
	

    puts("Checking for Visual Studio 2010...")
    abort("Can't find a Visual Studio 2010 environment!") if !ENV["VS100COMNTOOLS"]

    puts("Checking for Visual Studio 2008...")
    abort("Can't find a Visual Studio 2008 environment!") if !ENV["VS90COMNTOOLS"]

   	puts "Checking for devenv..."
	abort("Can't find valid devenv 2010 environment!") if !File.exists?("#{DEVENV2010}") 
	abort("Can't find valid devenv 2008 environment!") if !File.exists?("#{DEVENV2008}")
end

desc "Build C++ Samples"
task :build,[:variant] => [:setup] do |t, args|

	type = args[:variant][0]
	
	if type == nil
		abort("Must run build with a valid variant. E.g, rake build[debug]")
	end
	
	if type == "debug"
		t.invoke_in_scope('build_debug_2010')
	end
	
	if type == "release"
		#This var is set in the root Rakefile if we're building just a subrepo
		#gross, I know, but this way C++ samples will build against pre-built libs when
		#building a subrepo (i.e. you don't have to build sdk then build the samples)
		if ENV["PUREWEB_BUILD_SUBREPO"] == "true"			
			t.invoke_in_scope('build_release_solo_2010')
		else
			t.invoke_in_scope('build_release_2010')
		end
	end
end


desc "Stage the C++ Samples into #{PUREWEB_HOME}"
task :stage => [@stagedir] do |t|	
	t.invoke_in_scope('build', 'release') # Staging is done by the DeployServer.bat Post-Build
end

task :stageclean do	
	FileUtils.rm_r PUREWEB_HOME + '/apps/DDxService', :force => true
	FileUtils.rm_r PUREWEB_HOME + '/apps/ScribbleAppCpp', :force => true
	FileUtils.rm_r PUREWEB_HOME + '/apps/ScribbleAppQt', :force => true
end

desc "Test C++ Samples"
task :test do |t|	
end

# Does not clean everything - must blow away manually
desc "Clean All C++ Libraries"
task :clean,[:variant]  do |t, args|

	type = args[:variant][0]
	
	if type == nil
		abort("Must run clean with a valid variant. E.g, rake build[debug]")
	end	
	
	if type == "debug"
		t.invoke_in_scope('clean_debug_2010')
	end
	
	if type == "release"		
		t.invoke_in_scope('clean_release_2010')
	end
	
    clean_debris
end

#### Internal Tasks
task :build_debug_2010 => [:setup] do	
    sh("\"#{DEVENV2010}\" \"#{VS2010SLN}\" /Build \"DebugSamples|Any Cpu\" /Out #{BUILD_DIR.gsub("/","\\")}\\logs\\samples_debug_2010.log")    
end

task :build_release_2010 => [:setup] do
	sh("\"#{DEVENV2010}\" \"#{VS2010SLN}\" /Build \"ReleaseSamples|Any Cpu\" /Out #{BUILD_DIR.gsub("/","\\")}\\logs\\samples_release_2010.log")    
end

task :build_release => [:build_release_2010]

task :build_debug => [:build_debug_2010]

task :clean_debug_2010 => [:setup] do
   puts("Cleaning VS2010 Debug SDK...")
   sh("\"#{DEVENV2010}\" \"#{VS2010SLN}\" /Clean \"DebugSamples|Any Cpu\" /Out #{BUILD_DIR.gsub("/","\\")}\\logs\\samples_debug_2010.log")	
end

task :clean_release_2010 => [:setup] do
    puts("Cleaning VS2010 Release SDK...")
  sh("\"#{DEVENV2010}\" \"#{VS2010SLN}\" /Clean \"ReleaseSamples|Any Cpu\" /Out #{BUILD_DIR.gsub("/","\\")}\\logs\\samples_debug_2010.log")	
end

task :clean_release => [:clean_release_2010]

task :clean_debug => [:clean_debug_2010]

#The difference between these tasks below and those above, are that the ones below are based around DDxService.vcxproj and Scribble.vcxproj 
#Whereas the above are built around DDxServiceDebug.vcxproj and ScribbleDebug.vcxproj.  The difference between these pairs of projects
#is that the former refers to pre-built PureWeb components in DLLs found in PUREWEB_HOME, whereas the latter finds its references in 
#source.  The tasks below are used for building the samples that get uploaded to S3.
task :build_release_solo_2010 => [:setup] do	
	sh("\"#{DEVENV2010}\" \"#{VS2010_DDX_SLN}\" /Build \"Release|x64\" /Out #{BUILD_DIR.gsub("/","\\")}\\logs\\ddx_debug_solo_2010.log")	
	sh("\"#{DEVENV2010}\" \"#{VS2010_SCRIBBLE_SLN}\" /Build \"Release|x64\" /Out #{BUILD_DIR.gsub("/","\\")}\\logs\\scribble_debug_solo_2010.log")	
end

task :build_release_solo => [:build_release_solo_2010]

task :clean_release_solo_2010 => [:setup] do
  puts("Cleaning VS2010 Release SDK...")
  sh("\"#{DEVENV2010}\" \"#{VS2010_DDX_SLN}\" /Clean \"Release|x64\" /Out #{BUILD_DIR.gsub("/","\\")}\\logs\\ddx_debug_solo_2010.log")	
  sh("\"#{DEVENV2010}\" \"#{VS2010_SCRIBBLE_SLN}\" /Clean \"Release|x64\" /Out #{BUILD_DIR.gsub("/","\\")}\\logs\\scribble_debug_solo_2010.log")	
end

task :clean_release_solo => [:clean_release_solo_2010]

task :cleantest do |t|
	t.invoke_in_scope('clean', 'debug')
	t.invoke_in_scope('clean', 'release')	
end



