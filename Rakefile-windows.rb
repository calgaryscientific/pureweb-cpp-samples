

@ddxservicetarget = "Samples\\DDx\\DDxService"
@scribbletarget = "Samples\\Scribble\\ScribbleAppDebug_Cpp"

@sampletargets = "#{@ddxservicetarget};#{@scribbletarget}"
@samplecleantargets = "#{@ddxservicetarget}:clean;#{@scribbletarget}:clean"

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
		t.invoke_in_scope('build_release_2010')
	end
end


desc "Stage the C++ Samples into #{ENV['PUREWEB_HOME']}"
task :stage => [@stagedir] do |t|	
	t.invoke_in_scope('build', 'release') # Staging is done by the DeployServer.bat Post-Build
end

task :stageclean do	
	FileUtils.rm_r ENV['PUREWEB_HOME'] + '/apps/DDxService', :force => true
	FileUtils.rm_r ENV['PUREWEB_HOME'] + '/apps/ScribbleAppCpp', :force => true
	FileUtils.rm_r ENV['PUREWEB_HOME'] + '/apps/ScribbleAppQt', :force => true
end

desc "Test C++ Samples"
task :test do |t|	
end

desc "Package the C++ SDK"
task :package do
end

task :packageclean do
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

task :cleantest do |t|
	t.invoke_in_scope('clean', 'debug')
	t.invoke_in_scope('clean', 'release')	
end



