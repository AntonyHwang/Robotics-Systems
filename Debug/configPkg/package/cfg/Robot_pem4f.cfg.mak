# invoke SourceDir generated makefile for Robot.pem4f
Robot.pem4f: .libraries,Robot.pem4f
.libraries,Robot.pem4f: package/cfg/Robot_pem4f.xdl
	$(MAKE) -f /Users/AntonyHwang/Desktop/working/robotic_systems/src/makefile.libs

clean::
	$(MAKE) -f /Users/AntonyHwang/Desktop/working/robotic_systems/src/makefile.libs clean

