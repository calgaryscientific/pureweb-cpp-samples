scribbleapp_src = "ScribbleAppQt"

lib_dir = "#{BUILD_DIR}/Libs/C++/lib"

deps_src = FileList.new(
            "#{CSI_LIB}/CSI.Standard/lib/*",
            "#{CSI_LIB}/CSI.Typeless/lib/*",
            "#{CSI_LIB}/libjpeg-turbo/lib/*",
            "#{CSI_LIB}/ICU/lib/*.so*")

deps_src.exclude(/test/)

SERVICE_MGR_CFG = "#{STAGINGDIR}/services/service-manager-cfg"

#### Helper Functions

def install_qmake()
    case
    when OS.aptget?
        sh("#{SUDO} apt-get -y install qt4-qmake")
    when OS.yum?
        sh("#{SUDO} yum -y install qt4-devel")
        sh("#{SUDO} ln -s /usr/bin/qmake-qt4 /usr/bin/qmake")
    else
        abort("Can't find qmake!")
    end
end


#### Task definitions

desc "Setup the Linux build environment"
task :setup do
    puts("Checking for qmake...")
    sh("qmake --version") { |ok, res| install_qmake() unless ok }

    abort("JAVA_HOME is not set!") unless ENV["JAVA_HOME"]

    abort("PUREWEB_LIBS is not set!") unless ENV["PUREWEB_LIBS"]

#   puts "Checking for libuuid..."
#   case
#   when OS.yum?
#       sh("#{SUDO} yum -y install libuuid-devel")
#   end

end

#### Lib Dependencies

directory lib_dir

task :copy_deps => [lib_dir] do
    deps_src.each do |src|
        dst = src.pathmap("#{lib_dir}/%f")
        sh("cp -a #{src} #{dst}") if !File.exists?(dst) || File.mtime(src) > File.mtime(dst)
    end
end


desc "Build the ScribbleAppQt sample"
task :build_scribbleqt => [:setup, :copy_deps] do
    puts("Building Scribble example...")
    sh("cd #{scribbleapp_src} && qmake -makefile scribble.pro && make")
end

desc "Build the ScribbleAppQt solo sample"
task :build_scribbleqt_solo => [:setup] do
    puts("Building Scribble example...")
    sh("cd #{scribbleapp_src} && qmake -makefile scribble_solo.pro && make")
end

desc "Clean the ScribbleAppQt sample"
task :clean_scribbleqt do
    puts("Building Scribble example...")
    sh("cd #{scribbleapp_src} && make clean")
end

desc "Test C++ Samples"
task :test do |t|	
end

desc "Build all C++ samples"
task :build do |t|
  if ENV["PUREWEB_BUILD_SUBREPO"] == "true"
    t.invoke_in_scope('build_scribbleqt_solo')
  else 
    t.invoke_in_scope('build_scribbleqt')    
  end
end

desc "Clean all C++ samples"
task :clean => [:clean_scribbleqt]

desc "Deploy Samples"
task :deploy do 
  if ENV["PUREWEB_BUILD_SUBREPO"] == "true"
    t.invoke_in_scope('deploy_scribbleqt_solo')
  else 
    t.invoke_in_scope('deploy_scribbleqt')    
end 


desc "Stage the C++ samples"
task :stage do |t|
	t.invoke_in_scope('build')
  t.invoke_in_scope('deploy')
end

task :stageclean do
	FileUtils.rm @stagedir, :force => true
end

desc "Deploy Qt example"
task :deploy_scribbleqt do
  puts("Deploying Scribble example")
  FileUtils.touch("#{STAGINGDIR}/services/service_config.json")
  FileUtils.mkdir_p "#{STAGINGDIR}/apps/ScribbleAppQt"
  FileUtils.cp "#{scribbleapp_src}/debug/scribble", "#{STAGINGDIR}/apps/ScribbleAppQt"
  FileUtils.cp "#{scribbleapp_src}/service.json", "#{STAGINGDIR}/apps/ScribbleAppQt"

  if File.file?("#{SERVICE_MGR_CFG}")
    puts("Writing out config file")
    sh("#{SERVICE_MGR_CFG} -configFile #{STAGINGDIR}/services/service_config.json -action add -changeFile #{scribbleapp_src}/service.json") 
  end
end

desc "Deploy Qt example"
task :deploy_scribbleqt_solo do
  puts("Deploying Scribble solo example")
  FileUtils.mkdir_p "#{STAGINGDIR}/apps/ScribbleAppQt"
  FileUtils.cp "#{scribbleapp_src}/debug/scribble", "#{STAGINGDIR}/apps/ScribbleAppQt"
  FileUtils.cp "#{scribbleapp_src}/service.json", "#{STAGINGDIR}/apps/ScribbleAppQt"
end
