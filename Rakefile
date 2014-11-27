
require_relative("../../Rakefile-init")

if OS.windows?
    require_relative("./Rakefile-windows")
else
    require_relative("./Rakefile-linux")
end

desc "Build, Stage, Package & Test C++ Samples"
task :all => [:setup, :stage, :package, :test]

