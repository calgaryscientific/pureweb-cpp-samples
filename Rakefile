
require("../../Rakefile-init")

if OS.windows?
    require("./Rakefile-windows")
else
    require("./Rakefile-linux")
end

desc "Build and test C++ libraries"
task :all => [:build, :test]

