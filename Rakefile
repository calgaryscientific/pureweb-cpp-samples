require_relative("../../Rakefile-init")

projects = {
    "DDx" => "./DDxService",        
    "Scribble" => "./ScribbleAppCpp",
    "ScribbleQT" => "./ScribbleAppQt"
}

if OS.windows?
    require_relative("./Rakefile-windows")
else
    require_relative("./Rakefile-linux")
end

desc "Build, Stage, Package & Test C++ Samples"
task :all => [:setup, :stage, :package, :test]

desc "Package the C++ Samples"
task :package do
	if !Dir.exists?("#{PUREWEB_HOME}/../pkg")
		FileUtils.mkdir "#{PUREWEB_HOME}/../pkg"
	end

	projects.each do |name, project|
		archiveName = "pureweb-sample-CPP-service-#{name}"	
		if !Dir.glob("#{PUREWEB_HOME}/apps/#{project}/").empty?
            archive = OS.windows? ? "\"#{CSI_LIB}\\Tools\\7zip\\7z.exe\" a -tzip #{PUREWEB_HOME}\\..\\pkg\\#{archiveName}.zip #{PUREWEB_HOME}/apps/#{project}/*" :
                "zip -rj #{PUREWEB_HOME}/../pkg/#{archiveName}.zip #{PUREWEB_HOME}/apps/#{project}/"
			sh(archive)
	    end
	end				
end

task :packageclean do
	projects.each do |name, project|
		archiveName = "pureweb-sample-CPP-service-#{name}"	
		if File.exists?("#{PUREWEB_HOME}/../pkg/#{archiveName}.zip")
			puts "deleting #{PUREWEB_HOME}/../pkg/#{archiveName}.zip"
			File.delete "#{PUREWEB_HOME}/../pkg/#{archiveName}.zip"
		end
	end	
end

desc "Upload samples to S3"	
task :upload_to_s3 do
	projectname = File.basename(File.dirname(__FILE__))
	repo_source_description = `git describe --long`.strip().match(/^(?<version>.*?)(-(?<variant>.*?))?-(?<revision>.*?)-(?<hash>.*?)$/)
	version = repo_source_description['version']    

	projects.each do |name, project|    	   
	    puts ("Attempting to uploading #{project} to AWS S3")
		filename = "pureweb-sample-CPP-service-#{name}"
		puts "looking for #{PUREWEB_HOME}/../pkg/#{filename}.zip"
	    if File.exists?("#{PUREWEB_HOME}/../pkg/#{filename}.zip")
	        #upload to the versioned directory

	        branch = ENV["BUILD_BRANCH"]

			print "Current branch is #{branch}"

			if branch != "master"
				sh("aws s3 cp #{PUREWEB_HOME}/../pkg/#{filename}.zip s3://base.pureweb-apps/branches/#{branch}/#{name}/#{version}/#{repo_source_description}/#{BUILD_OS}/#{filename}.zip --region us-west-2")
			else	
	        	sh("aws s3 cp #{PUREWEB_HOME}/../pkg/#{filename}.zip s3://base.pureweb-apps/releases/#{name}/#{version}/#{repo_source_description}/#{BUILD_OS}/#{filename}.zip --region us-west-2")

	        	#given that this should only ever be run from a build machine, we can assume that this build also represents the 'latest' build
	        	sh("aws s3 cp s3://base.pureweb-apps/releases/#{name}/#{version}/#{repo_source_description}/#{BUILD_OS}/#{filename}.zip s3://base.pureweb-apps/releases/#{name}/latest/#{BUILD_OS}/#{filename}.zip --region us-west-2")
	        end
	    else
	        puts("No file found.  Skipping upload.")
	    end	
	end
end

task :stageTomcat do
    dir = File.dirname(__FILE__)	
    FileUtils.cp dir + "/DDx/tomcat/DDxCpp-plugin.xml", PUREWEB_HOME + "/tomcat-server/conf"
    FileUtils.cp dir + "/Scribble/tomcat/ScribbleCpp-plugin.xml", PUREWEB_HOME + "/tomcat-server/conf"
end