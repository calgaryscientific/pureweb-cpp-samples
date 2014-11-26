
require_relative("../../Rakefile-init")

if OS.windows?
    require_relative("./Rakefile-windows")
else
    require_relative("./Rakefile-linux")
end

desc "Build and test C++ libraries"
task :all => [:build, :test]

