

@ddxservicetarget = "Samples\\DDx\\DDxService"
@scribbletarget = "Samples\\Scribble\\ScribbleAppDebug_Cpp"

@sampletargets = "#{DDxServiceTarget};#{ScribbleTarget}"
@samplecleantargets = "#{DDxServiceTarget}:clean;#{ScribbleTarget}:clean"

@stagedir = "#{BUILD_DIR}/stage/sdk/Libs/C++"

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

    puts("Checking for Visual Studio 2010...")
    abort("Can't find a Visual Studio 2010 environment!") if !ENV["VS100COMNTOOLS"]

    puts("Checking for Visual Studio 2008...")
    abort("Can't find a Visual Studio 2008 environment!") if !ENV["VS90COMNTOOLS"]

    puts("Checking for msbuild...")
    sh("#{MSBUILD} /version") { |ok, res| abort("Can't find msbuild!") if !ok }
end

# Does not clean everything - must blow away manually
desc "Clean All C++ libraries"
task :clean,[:variant]  do |t, args|

	type = args[:variant][0]
	
	if type == nil
		abort("Must run clean with a valid variant. E.g, rake build[debug]")
	end
	
	Dir.chdir(File.dirname(__FILE__))
	
	if type == "debug"
		sh("rake clean_debug_2010")
	end
	
	if type == "release"
		sh("rake clean_release_2010")
	end
	
    clean_debris
end



#### Internal Tasks
task :build_debug_2010 => [:setup] do
	sh("#{MSBUILD} #{VS2010SLN} /m /t:#{SampleTargets} /p:Configuration=DebugCpp;BuildProjectReferences=false")
end

task :build_release_2010 => [:setup] do
	sh("#{MSBUILD} #{VS2010SLN} /m /t:#{SampleTargets} /p:Configuration=ReleaseCpp;BuildProjectReferences=false")
end

task :build_release => [:build_release_2010]


task :build_debug => [:build_debug_2010]

task :build,[:variant] => [:deps] do |t, args|

	type = args[:variant][0]
	
	if type == nil
		abort("Must run build with a valid variant. E.g, rake build[debug]")
	end
	
	Dir.chdir(File.dirname(__FILE__))
	
	if type == "debug"
		sh("rake build_debug_2010")
	end
	
	if type == "release"
		sh("rake build_release_2010")
	end
end

desc "Test SDK sample C++ libraries"
task :test do
end



task :clean_debug_2010 => [:setup] do
    puts("Cleaning VS2010 Debug SDK...")
   sh("#{MSBUILD} #{VS2010SLN} /m /t:#{@samplecleantargets} /p:Configuration=DebugCpp")
end


task :clean_release_2010 => [:setup] do
    puts("Cleaning VS2010 Release SDK...")
  sh("#{MSBUILD} #{VS2010SLN} /m /t:#{@samplecleantargets} /p:Configuration=ReleaseCpp")
end


task :clean_release => [:clean_release_2010]



task :clean_debug => [:clean_debug_2010]

task :cleantest do |t|
	t.invoke_in_scope('clean', 'debug')
	t.invoke_in_scope('clean', 'release')	
end



