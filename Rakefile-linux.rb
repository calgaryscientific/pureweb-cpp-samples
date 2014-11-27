SCRIBBLEAPP_SRC = "ScribbleAppQt"

LIB_DIR = "#{BUILD_DIR}/Libs/C++/lib"

DEPS_SRC = FileList.new(
            "#{CSI_LIB}/CSI.Standard/lib/*",
            "#{CSI_LIB}/CSI.Typeless/lib/*",
            "#{CSI_LIB}/libjpeg-turbo/lib/*",
            "#{CSI_LIB}/ICU/lib/*.so*")

DEPS_SRC.exclude(/test/)

SERVICE_MGR_CFG = "#{STAGINGDIR}/bin/service-manager-cfg"

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

#   puts "Checking for libuuid..."
#   case
#   when OS.yum?
#       sh("#{SUDO} yum -y install libuuid-devel")
#   end

end

#### Lib Dependencies

directory LIB_DIR

task :copy_deps => [LIB_DIR] do
    DEPS_SRC.each do |src|
        dst = src.pathmap("#{LIB_DIR}/%f")
        sh("cp -a #{src} #{dst}") if !File.exists?(dst) || File.mtime(src) > File.mtime(dst)
    end
end


desc "Build the ScribbleAppQt sample"
task :build_scribbleqt => [:setup, :copy_deps] do
    puts("Building Scribble example...")
    sh("cd #{SCRIBBLEAPP_SRC} && qmake -makefile scribble.pro && make")
end

desc "Clean the ScribbleAppQt sample"
task :clean_scribbleqt do
    puts("Building Scribble example...")
    sh("cd #{SCRIBBLEAPP_SRC} && make clean")
end

desc "Build all C++ samples"
task :build => [:build_scribbleqt]

desc "Clean all C++ samples"
task :clean => [:clean_scribbleqt]

desc "Deploy Samples"
task :deploy => [:deploy_scribbleqt]


desc "Deploy Qt example"
task :deploy_scribbleqt do
  puts("Deploying Scribble example")
  FileUtils.touch("#{STAGINGDIR}/bin/service_config.json")
  FileUtils.mkdir_p "#{STAGINGDIR}/bin/ScribbleAppQt"
  FileUtils.cp "#{SCRIBBLEAPP_SRC}/debug/scribble", "#{STAGINGDIR}/bin/ScribbleAppQt"
  
  if File.file?("#{SERVICE_MGR_CFG}")
    puts("Writing out config file")
    sh("#{SERVICE_MGR_CFG} -configFile #{STAGINGDIR}/bin/service_config.json -action add -changeFile #{SCRIBBLEAPP_SRC}/service.json") 
  end
end
