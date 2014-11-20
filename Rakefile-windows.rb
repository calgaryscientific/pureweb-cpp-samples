
MSBUILD = "C:/Windows/Microsoft.NET/Framework/v4.0.30319/msbuild"
VS2010SLN = "#{PWROOT}/PureWeb2010.sln"
VS2008SLN = "#{PWROOT}/PureWeb2008.sln"

DDxServiceTarget = "Samples\\DDx\\DDxService"
ScribbleTarget = "Samples\\Scribble\\ScribbleAppDebug_Cpp"

SampleTargets = "#{DDxServiceTarget};#{ScribbleTarget}"
SampleCleanTargets = "#{DDxServiceTarget}:clean;#{ScribbleTarget}:clean"

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


desc "Build VS2010 C++ SDK libraries"
task :build_debug_2010 => [:setup] do
	sh("#{MSBUILD} #{VS2010SLN} /m /t:#{SampleTargets} /p:Configuration=DebugCpp;BuildProjectReferences=false")
end


desc "Build VS2010 Release C++ libraries"
task :build_release_2010 => [:setup] do
	sh("#{MSBUILD} #{VS2010SLN} /m /t:#{SampleTargets} /p:Configuration=ReleaseCpp;BuildProjectReferences=false")
end

desc "Build only release libraries that are commonly used by devs"
task :build_release => [:build_release_2010]

desc "Build only debug libraries that are commonly used by devs"
task :build_debug => [:build_debug_2010]

desc "Build All C++ libraries"
task :build,[:variant] => [:deps] do |t, args|

	type = args[:variant][0]
	
	if type == nil
		abort("Must run build with a valid variant. E.g, rake build[debug]")
	end
	
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


desc "Clean VS2010 Debug C++ libraries"
task :clean_debug_2010 => [:setup] do
    puts("Cleaning VS2010 Debug SDK...")
   sh("#{MSBUILD} #{VS2010SLN} /m /t:#{SampleCleanTargets} /p:Configuration=DebugCpp")
end

desc "Clean VS2010 Release C++ libraries"
task :clean_release_2010 => [:setup] do
    puts("Cleaning VS2010 Release SDK...")
  sh("#{MSBUILD} #{VS2010SLN} /m /t:#{SampleCleanTargets} /p:Configuration=ReleaseCpp")
end

desc "Clean only release libraries that are commonly used by devs"
task :clean_release => [:clean_release_2010]


desc "Clean only debug libraries that are commonly used by devs"
task :clean_debug => [:clean_debug_2010]


# Does not clean everything - must blow away manually
desc "Clean All C++ libraries"
task :clean,[:variant]  do |t, args|

	type = args[:variant][0]
	
	if type == nil
		abort("Must run clean with a valid variant. E.g, rake build[debug]")
	end
	
	if type == "debug"
		sh("rake clean_debug_2010")
	end
	
	if type == "release"
		sh("rake clean_release_2010")
	end
	
    clean_debris
end

desc "Clean All the C++ tests"
task :cleantest do
	Rake::Task[:clean].invoke "debug"
	Rake::Task[:clean].invoke "release"
end



